/*******************************************************************************
 * Copyright (c) 2012-2014, The Microsystems Design Labratory (MDL)
 * Department of Computer Science and Engineering, The Pennsylvania State
 *University
 *
 * Copyright (c) 2019, Chair for Compiler Construction
 * Department of Computer Science, TU Dresden
 * All rights reserved.
 *
 * This source code is part of NVMain - A cycle accurate timing, bit accurate
 * energy simulator for both volatile (e.g., DRAM) and non-volatile memory
 * (e.g., PCRAM).
 *
 * The original NVMain doesn't support simulating RaceTrack memory.
 * This current version, which we call RTSim, enables RTM simulation.
 *
 * The source code is free and you can redistribute and/or
 * modify it by providing that the following conditions are met:
 *
 *  1) Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2) Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author list:
 *   Matt Poremba    ( Email: mrp5060 at psu dot edu
 *                     Website: http://www.cse.psu.edu/~poremba/ )
 *   Tao Zhang       ( Email: tzz106 at cse dot psu dot edu
 *                     Website: http://www.cse.psu.edu/~tzz106 )
 *
 *   Asif Ali Khan   ( Email: asif_ali.khan@tu-dresden.de
 *
 *******************************************************************************/

#include "src/MemoryController.h"

#include "include/NVMainRequest.h"
#include "include/NVMHelpers.h"
#include "Interconnect/InterconnectFactory.h"
#include "src/EventQueue.h"
#include "src/Interconnect.h"
#include "src/Rank.h"
#include "src/SubArray.h"

#include <algorithm>
#include <cassert>
#include <csignal>
#include <cstdlib>
#include <functional>
#include <limits>
#include <sstream>

using namespace NVM;

/*
 * Returns true if the given request has been issued, false otherwise
 *
 * @param request Request to check
 * @return True if request has been issued, false otherwise
 */
bool WasIssued(NVMainRequest* request);

bool WasIssued(NVMainRequest* request) {
    return (request->flags & NVMainRequest::FLAG_ISSUED);
}

MemoryController::MemoryController() : reqMaker(this), reqFinder(this) {
    transactionQueues = NULL;
    transactionQueueCount = 0;
    commandQueues = NULL;
    commandQueueCount = 0;

    lastCommandWake = 0;
    wakeupCount = 0;
    lastIssueCycle = 0;

    starvationThreshold = 4;
    subArrayNum = 1;
    activateQueued = NULL;
    delayedRefreshCounter = NULL;

    curQueue = 0;
    nextRefreshRank = 0;
    nextRefreshBank = 0;

    handledRefresh = std::numeric_limits<ncycle_t>::max();
}

MemoryController::~MemoryController() {
    for (ncounter_t i = 0; i < p->RANKS; i++) {
        delete[] activateQueued[i];
        delete[] bankNeedRefresh[i];
        delete[] refreshQueued[i];
    }

    delete[] commandQueues;
    delete[] activateQueued;
    delete[] bankNeedRefresh;
    delete[] rankPowerDown;

    if (p->UseRefresh) {
        for (ncounter_t i = 0; i < p->RANKS; i++) {
            delete[] delayedRefreshCounter[i];
        }
    }

    delete[] delayedRefreshCounter;
}

void MemoryController::InitQueues(unsigned int numQueues) {
    if (transactionQueues != NULL) delete[] transactionQueues;

    transactionQueues = new NVMTransactionQueue[numQueues];
    transactionQueueCount = numQueues;

    for (unsigned int i = 0; i < numQueues; i++) transactionQueues[i].clear();
}

void MemoryController::Cycle(ncycle_t) {
    bool scheduled = false;
    ncycle_t nextWakeup = GetEventQueue()->GetCurrentCycle() + 1;

    if (GetEventQueue()->FindEvent(EventCycle, this, NULL, nextWakeup)) return;

    for (ncounter_t queueIdx = 0; queueIdx < commandQueueCount; queueIdx++) {
        if (EffectivelyEmpty(queueIdx) && TransactionAvailable(queueIdx)) {
            GetEventQueue()->InsertEvent(EventCycle, this, nextWakeup, NULL,
                                         transactionQueuePriority);

            scheduled = true;
            break;
        }

        if (scheduled) {
            break;
        }
    }
}

void MemoryController::Prequeue(ncounter_t queueNum, NVMainRequest* request) {
    assert(queueNum < transactionQueueCount);

    transactionQueues[queueNum].push_front(request);
}

void MemoryController::Enqueue(ncounter_t queueNum, NVMainRequest* request) {
    ncounter_t channel, rank, bank, row, col, subarray;

    GetDecoder()->Translate(request->address.GetPhysicalAddress(), &row, &col,
                            &bank, &rank, &channel, &subarray);
    channel = request->address.GetChannel();
    request->address.SetTranslatedAddress(row, col, bank, rank, channel,
                                          subarray);

    assert(queueNum < transactionQueueCount);

    transactionQueues[queueNum].push_back(request);

    ncounter_t queueId = GetCommandQueueId(request->address);

    if (EffectivelyEmpty(queueId)) {
        ncycle_t nextWakeup = GetEventQueue()->GetCurrentCycle();

        if (GetEventQueue()->FindEvent(EventCycle, this, NULL, nextWakeup) ==
            NULL) {
            GetEventQueue()->InsertEvent(EventCycle, this, nextWakeup, NULL,
                                         transactionQueuePriority);
        }
    }
}

bool MemoryController::TransactionAvailable(ncounter_t queueId) {
    bool rv = false;

    for (ncounter_t queueIdx = 0; queueIdx < transactionQueueCount;
         queueIdx++) {
        std::list<NVMainRequest*>::iterator it;

        for (it = transactionQueues[queueIdx].begin();
             it != transactionQueues[queueIdx].end(); it++) {
            if (GetCommandQueueId((*it)->address) == queueId) {
                rv = true;
                break;
            }
        }
    }

    return rv;
}

void MemoryController::ScheduleCommandWake() {
    ncycle_t nextWakeup = NextIssuable(NULL);

    bool nextWakeupScheduled = GetEventQueue()->FindCallback(
        this, (CallbackPtr) &MemoryController::CommandQueueCallback, nextWakeup,
        NULL, commandQueuePriority);

    if (!nextWakeupScheduled) {
        GetEventQueue()->InsertCallback(
            this, (CallbackPtr) &MemoryController::CommandQueueCallback,
            nextWakeup, NULL, commandQueuePriority);
    }
}

void MemoryController::CommandQueueCallback(void*) {
    ncycle_t realSteps = GetEventQueue()->GetCurrentCycle() - lastCommandWake;
    lastCommandWake = GetEventQueue()->GetCurrentCycle();

    ncycle_t nextWakeup = NextIssuable(NULL);
    wakeupCount++;

    bool nextWakeupScheduled = GetEventQueue()->FindCallback(
        this, (CallbackPtr) &MemoryController::CommandQueueCallback, nextWakeup,
        NULL, commandQueuePriority);

    if (!nextWakeupScheduled &&
        nextWakeup != std::numeric_limits<ncycle_t>::max()) {
        GetEventQueue()->InsertCallback(
            this, (CallbackPtr) &MemoryController::CommandQueueCallback,
            nextWakeup, NULL, commandQueuePriority);
    }

    CycleCommandQueues();

    GetChild()->Cycle(realSteps);
}

void MemoryController::RefreshCallback(void* data) {
    NVMainRequest* request = reinterpret_cast<NVMainRequest*>(data);

    ncycle_t realSteps = GetEventQueue()->GetCurrentCycle() - lastCommandWake;
    lastCommandWake = GetEventQueue()->GetCurrentCycle();
    wakeupCount++;

    ProcessRefreshPulse(request);
    HandleRefresh();

    GetChild()->Cycle(realSteps);
}

void MemoryController::CleanupCallback(void*) {
    for (ncycle_t queueId = 0; queueId < commandQueueCount; queueId++) {
        commandQueues[queueId].erase(
            std::remove_if(commandQueues[queueId].begin(),
                           commandQueues[queueId].end(), WasIssued),
            commandQueues[queueId].end());
    }
}

bool MemoryController::RequestComplete(NVMainRequest* request) {
    if (request->owner == this) {
        delete request;
    } else {
        return GetParent()->RequestComplete(request);
    }

    return true;
}

bool MemoryController::IsIssuable(NVMainRequest*, FailReason*) { return true; }

void MemoryController::SetMappingScheme() {
    GetDecoder()->GetTranslationMethod()->SetAddressMappingScheme(
        p->AddressMappingScheme);
}

void MemoryController::SetConfig(Config* conf, bool createChildren) {
    Params* params = new Params();
    params->SetParams(conf);
    SetParams(params);

    if (createChildren) {
        uint64_t channels, ranks, banks, rows, cols, subarrays;

        AddressTranslator* mcAT =
            DecoderFactory::CreateDecoderNoWarn(conf->GetString("Decoder"));
        mcAT->SetDefaultField(NO_FIELD);
        mcAT->SetConfig(conf, createChildren);
        SetDecoder(mcAT);

        GetParent()
            ->GetTrampoline()
            ->GetDecoder()
            ->GetTranslationMethod()
            ->GetCount(&rows, &cols, &banks, &ranks, &channels, &subarrays);

        if (conf->KeyExists("MATHeight")) {
            rows = p->MATHeight;
            subarrays = p->ROWS / p->MATHeight;
        } else {
            rows = p->ROWS;
            subarrays = 1;
        }
        cols = p->COLS;
        banks = p->BANKS;
        ranks = p->RANKS;

        TranslationMethod* method = new TranslationMethod();

        method->SetBitWidths(NVM::mlog2(rows), NVM::mlog2(cols),
                             NVM::mlog2(banks), NVM::mlog2(ranks),
                             NVM::mlog2(channels), NVM::mlog2(subarrays));
        method->SetCount(rows, cols, banks, ranks, channels, subarrays);
        mcAT->SetTranslationMethod(method);

        std::stringstream confString;

        auto memory = InterconnectFactory::CreateInterconnect(
            conf->GetString("INTERCONNECT"));

        confString.str("");
        confString << StatName() << ".channel" << GetID();
        memory->StatName(confString.str());

        memory->SetParent(this);
        AddChild(memory);

        std::cout << '1';
        memory->SetConfig(conf, createChildren);
        memory->RegisterStats();

        SetMappingScheme();
    }

    std::cout << StatName() << " capacity is "
              << ((p->ROWS * p->COLS * p->tBURST * p->RATE * p->BusWidth *
                   p->BANKS * p->RANKS) /
                  (8 * 1024))
              << " KB." << std::endl;

    if (conf->KeyExists("MATHeight")) {
        subArrayNum = p->ROWS / p->MATHeight;
    } else {
        subArrayNum = 1;
    }

    queueModel = PerBankQueues;
    commandQueueCount = p->RANKS * p->BANKS;
    if (conf->KeyExists("QueueModel")) {
        if (conf->GetString("QueueModel") == "PerRank") {
            queueModel = PerRankQueues;
            commandQueueCount = p->RANKS;
        } else if (conf->GetString("QueueModel") == "PerBank") {
            queueModel = PerBankQueues;
            commandQueueCount = p->RANKS * p->BANKS;
        } else if (conf->GetString("QueueModel") == "PerSubArray") {
            queueModel = PerSubArrayQueues;
            commandQueueCount = p->RANKS * p->BANKS * subArrayNum;
        }
    }

    commandQueues = new std::deque<NVMainRequest*>[commandQueueCount];
    activateQueued = new bool*[p->RANKS];
    refreshQueued = new bool*[p->RANKS];
    starvationCounters = SubArrayCounter(p, conf);
    activeSubArrays = SubArrayCounter(p, conf);
    activeRow = SubArrayCounter(p, conf, p->ROWS);
    activeMuxedRow = SubArrayCounter(p, conf, p->ROWS);
    rankPowerDown = new bool[p->RANKS];

    for (ncounter_t i = 0; i < p->RANKS; i++) {
        activateQueued[i] = new bool[p->BANKS];
        refreshQueued[i] = new bool[p->BANKS];

        if (p->UseLowPower) rankPowerDown[i] = p->InitPD;
        else rankPowerDown[i] = false;

        for (ncounter_t j = 0; j < p->BANKS; j++) {
            activateQueued[i][j] = false;
            refreshQueued[i][j] = false;
        }
    }

    bankNeedRefresh = new bool*[p->RANKS];
    for (ncounter_t i = 0; i < p->RANKS; i++) {
        bankNeedRefresh[i] = new bool[p->BANKS];
        for (ncounter_t j = 0; j < p->BANKS; j++) {
            bankNeedRefresh[i][j] = false;
        }
    }

    delayedRefreshCounter = new ncounter_t*[p->RANKS];

    if (p->UseRefresh) {
        assert(p->BanksPerRefresh <= p->BANKS);
        assert(p->BanksPerRefresh != 0);

        m_refreshBankNum = p->BANKS / p->BanksPerRefresh;
        m_tREFI = p->tREFW / (p->ROWS / p->RefreshRows);

        ncycle_t m_refreshSlice = m_tREFI / (p->RANKS * m_refreshBankNum);

        for (ncounter_t i = 0; i < p->RANKS; i++) {
            delayedRefreshCounter[i] = new ncounter_t[m_refreshBankNum];

            for (ncounter_t j = 0; j < m_refreshBankNum; j++) {
                delayedRefreshCounter[i][j] = 0;

                ncounter_t refreshBankHead = j * p->BanksPerRefresh;
                NVMainRequest* refreshPulse = reqMaker.makeRefreshRequest(
                    0, 0, refreshBankHead, i, id, 0);
                ncycle_t offset = (i * m_refreshBankNum + j) * m_refreshSlice;

                GetEventQueue()->InsertCallback(
                    this, (CallbackPtr) &MemoryController::RefreshCallback,
                    GetEventQueue()->GetCurrentCycle() + m_tREFI + offset,
                    reinterpret_cast<void*>(refreshPulse), refreshPriority);
            }
        }
    }

    if (p->PrintConfig) conf->Print();

    SetDebugName("MemoryController", conf);
}

void MemoryController::RegisterStats() {
    AddStat(simulation_cycles);
    AddStat(wakeupCount);
}

bool MemoryController::NeedRefresh(const ncounter_t bank, const uint64_t rank) {
    bool rv = false;

    if (p->UseRefresh)
        if (delayedRefreshCounter[rank][bank / p->BanksPerRefresh] >=
            p->DelayedRefreshThreshold)
            rv = true;

    return rv;
}

void MemoryController::SetRefresh(const ncounter_t bank, const uint64_t rank) {
    ncounter_t bankHead = (bank / p->BanksPerRefresh) * p->BanksPerRefresh;

    for (ncounter_t i = 0; i < p->BanksPerRefresh; i++)
        bankNeedRefresh[rank][bankHead + i] = true;
}

void MemoryController::ResetRefresh(const ncounter_t bank,
                                    const uint64_t rank) {
    ncounter_t bankHead = (bank / p->BanksPerRefresh) * p->BanksPerRefresh;

    for (ncounter_t i = 0; i < p->BanksPerRefresh; i++)
        bankNeedRefresh[rank][bankHead + i] = false;
}

void MemoryController::ResetRefreshQueued(const ncounter_t bank,
                                          const ncounter_t rank) {
    ncounter_t bankHead = (bank / p->BanksPerRefresh) * p->BanksPerRefresh;

    for (ncounter_t i = 0; i < p->BanksPerRefresh; i++) {
        assert(refreshQueued[rank][bankHead + i]);
        refreshQueued[rank][bankHead + i] = false;
    }
}

void MemoryController::IncrementRefreshCounter(const ncounter_t bank,
                                               const uint64_t rank) {
    ncounter_t bankGroupID = bank / p->BanksPerRefresh;

    delayedRefreshCounter[rank][bankGroupID]++;
}

void MemoryController::DecrementRefreshCounter(const ncounter_t bank,
                                               const uint64_t rank) {
    ncounter_t bankGroupID = bank / p->BanksPerRefresh;

    delayedRefreshCounter[rank][bankGroupID]--;
}

bool MemoryController::HandleRefresh() {
    for (ncounter_t rankIdx = 0; rankIdx < p->RANKS; rankIdx++) {
        ncounter_t i = (nextRefreshRank + rankIdx) % p->RANKS;

        for (ncounter_t bankIdx = 0; bankIdx < m_refreshBankNum; bankIdx++) {
            ncounter_t j =
                (nextRefreshBank + bankIdx * p->BanksPerRefresh) % p->BANKS;
            FailReason fail;

            if (NeedRefresh(j, i)) {
                NVMainRequest* cmdRefresh =
                    reqMaker.makeRefreshRequest(0, 0, j, i, id, 0);

                if (p->UsePrecharge) {
                    for (ncounter_t tmpBank = 0; tmpBank < p->BanksPerRefresh;
                         tmpBank++) {
                        ncounter_t refBank = (tmpBank + j) % p->BANKS;
                        ncounter_t queueId = GetCommandQueueId(
                            NVMAddress(0, 0, refBank, i, 0, 0));

                        if (activateQueued[i][refBank] == true) {
                            NVMainRequest* cmdRefPre =
                                reqMaker.makePrechargeAllRequest(0, 0, refBank,
                                                                 i, id, 0);

                            commandQueues[queueId].push_back(cmdRefPre);
                            activeSubArrays.clear(i, refBank);
                            activeRow.clear(i, refBank, p->ROWS);
                            activeMuxedRow.clear(i, refBank, p->ROWS);
                            activateQueued[i][refBank] = false;
                        }
                    }
                }

                ncounter_t queueId =
                    GetCommandQueueId(NVMAddress(0, 0, j, i, 0, 0));

                cmdRefresh->issueCycle = GetEventQueue()->GetCurrentCycle();
                commandQueues[queueId].push_back(cmdRefresh);

                for (ncounter_t tmpBank = 0; tmpBank < p->BanksPerRefresh;
                     tmpBank++) {
                    ncounter_t refBank = (tmpBank + j) % p->BANKS;

                    refreshQueued[i][refBank] = true;
                }

                DecrementRefreshCounter(j, i);

                if (!NeedRefresh(j, i)) ResetRefresh(j, i);

                nextRefreshBank += p->BanksPerRefresh;
                if (nextRefreshBank >= p->BANKS) {
                    nextRefreshBank = 0;
                    nextRefreshRank++;

                    if (nextRefreshRank == p->RANKS) nextRefreshRank = 0;
                }

                handledRefresh = GetEventQueue()->GetCurrentCycle();

                ScheduleCommandWake();

                return true;
            }
        }
    }
    return false;
}

void MemoryController::ProcessRefreshPulse(NVMainRequest* refresh) {
    assert(refresh->type == REFRESH);

    ncounter_t rank, bank;
    refresh->address.GetTranslatedAddress(NULL, NULL, &bank, &rank, NULL, NULL);

    IncrementRefreshCounter(bank, rank);

    if (NeedRefresh(bank, rank)) SetRefresh(bank, rank);

    GetEventQueue()->InsertCallback(
        this, (CallbackPtr) &MemoryController::RefreshCallback,
        GetEventQueue()->GetCurrentCycle() + m_tREFI,
        reinterpret_cast<void*>(refresh), refreshPriority);
}

bool MemoryController::IsRefreshBankQueueEmpty(const ncounter_t bank,
                                               const uint64_t rank) {
    ncounter_t bankHead = (bank / p->BanksPerRefresh) * p->BanksPerRefresh;

    for (ncounter_t i = 0; i < p->BanksPerRefresh; i++) {
        ncounter_t queueId =
            GetCommandQueueId(NVMAddress(0, 0, bankHead + i, rank, 0, 0));
        if (!EffectivelyEmpty(queueId)) {
            return false;
        }
    }

    return true;
}

void MemoryController::PowerDown(const ncounter_t& rankId) {
    OpType pdOp = POWERDOWN_PDPF;
    if (p->PowerDownMode == "SLOWEXIT") pdOp = POWERDOWN_PDPS;
    else if (p->PowerDownMode == "FASTEXIT") pdOp = POWERDOWN_PDPF;
    else std::cerr << "NVMain Error: Undefined low power mode" << std::endl;

    NVMainRequest* powerdownRequest =
        reqMaker.makePowerdownRequest(pdOp, id, rankId);

    NVMObject* child;
    FindChildType(powerdownRequest, Rank, child);
    Rank* pdRank = dynamic_cast<Rank*>(child);

    if (pdRank->Idle() == false) {
        delete powerdownRequest;

        pdOp = POWERDOWN_PDA;
        powerdownRequest = reqMaker.makePowerdownRequest(pdOp, id, rankId);
    }

    if (RankQueueEmpty(rankId) && GetChild()->IsIssuable(powerdownRequest)) {
        GetChild()->IssueCommand(powerdownRequest);
        rankPowerDown[rankId] = true;
    } else {
        delete powerdownRequest;
    }
}

void MemoryController::PowerUp(const ncounter_t& rankId) {
    NVMainRequest* powerupRequest = reqMaker.makePowerupRequest(rankId, id);

    if (RankQueueEmpty(rankId) == false &&
        GetChild()->IsIssuable(powerupRequest)) {
        GetChild()->IssueCommand(powerupRequest);
        rankPowerDown[rankId] = false;
    } else {
        delete powerupRequest;
    }
}

void MemoryController::SetID(unsigned int id) { this->id = id; }

unsigned int MemoryController::GetID() { return this->id; }

bool MemoryController::IsLastRequest(
    std::list<NVMainRequest*>& transactionQueue, NVMainRequest* request) {
    bool rv = true;

    if (p->ClosePage == 0) {
        rv = false;
    } else if (p->ClosePage == 1) {
        ncounter_t mRank, mBank, mRow, mSubArray;
        request->address.GetTranslatedAddress(&mRow, NULL, &mBank, &mRank, NULL,
                                              &mSubArray);
        std::list<NVMainRequest*>::iterator it;

        for (it = transactionQueue.begin(); it != transactionQueue.end();
             it++) {
            ncounter_t rank, bank, row, subarray;

            (*it)->address.GetTranslatedAddress(&row, NULL, &bank, &rank, NULL,
                                                &subarray);
            if (rank == mRank && bank == mBank && row == mRow &&
                subarray == mSubArray) {
                rv = false;
                break;
            }
        }
    }

    return rv;
}

bool MemoryController::bankIsActivated(NVMainRequest* req) {
    ncounter_t rank, bank, row, subarray, col;

    req->address.GetTranslatedAddress(&row, &col, &bank, &rank, NULL,
                                      &subarray);
    ncounter_t muxLevel = static_cast<ncounter_t>(col / p->RBSize);
    ncounter_t queueId = GetCommandQueueId(req->address);

    return activateQueued[rank][bank];
}

bool MemoryController::handleCachedRequest(NVMainRequest* req) {
    ncounter_t rank, bank, row, subarray, col;

    req->address.GetTranslatedAddress(&row, &col, &bank, &rank, NULL,
                                      &subarray);

    ncounter_t muxLevel = static_cast<ncounter_t>(col / p->RBSize);
    ncounter_t queueId = GetCommandQueueId(req->address);

    NVMainRequest* cachedRequest = reqMaker.makeCachedRequest(req);

    FailReason reason;
    if (GetChild()->IsIssuable(cachedRequest, &reason)) {
        if (!activateQueued[rank][bank] || !activeSubArrays[req] ||
            activeRow[req] != row || activeMuxedRow[req] != muxLevel) {
            enqueueRequest(req);

            delete cachedRequest;

            return true;
        }
    }

    delete cachedRequest;
    return false;
}

bool MemoryController::rowIsActivated(NVMainRequest* req) {
    ncounter_t rank, bank, row, subarray, col;

    req->address.GetTranslatedAddress(&row, &col, &bank, &rank, NULL,
                                      &subarray);
    ncounter_t muxLevel = static_cast<ncounter_t>(col / p->RBSize);
    ncounter_t queueId = GetCommandQueueId(req->address);

    return activeSubArrays[req] && activeRow[req] == row &&
           activeMuxedRow[req] == muxLevel;
}

void MemoryController::enqueueActivate(NVMainRequest* req) {
    ncounter_t rank, bank, row, subarray, col;

    req->address.GetTranslatedAddress(&row, &col, &bank, &rank, NULL,
                                      &subarray);
    NVMainRequest* actRequest = reqMaker.makeActivateRequest(req);
    SubArray* writingArray = FindChild(req, SubArray);
    ncounter_t queueId = GetCommandQueueId(req->address);
    actRequest->flags |= (writingArray != NULL && writingArray->IsWriting())
                             ? NVMainRequest::FLAG_PRIORITY
                             : 0;
    commandQueues[queueId].push_back(actRequest);

    ncounter_t muxLevel = static_cast<ncounter_t>(col / p->RBSize);
    activateQueued[rank][bank] = true;
    activeSubArrays.increment(req);
    activeRow[req] = row;
    activeMuxedRow[req] = muxLevel;
    starvationCounters.clear(req);
}

void MemoryController::enqueueShift(NVMainRequest* req) {
    SubArray* writingArray = FindChild(req, SubArray);
    ncounter_t queueId = GetCommandQueueId(req->address);

    if (p->MemIsRTM) {
        NVMainRequest* shiftRequest = reqMaker.makeShiftRequest(
            req); // Place a shift request before the actual
                  // read/write on the command queue
        shiftRequest->flags |=
            (writingArray != NULL && writingArray->IsWriting())
                ? NVMainRequest::FLAG_PRIORITY
                : 0;
        commandQueues[queueId].push_back(shiftRequest);
    }
}

void MemoryController::enqueueRequest(NVMainRequest* req) {
    ncounter_t queueId = GetCommandQueueId(req->address);
    req->issueCycle = GetEventQueue()->GetCurrentCycle();
    commandQueues[queueId].push_back(req);
}

void MemoryController::enqueueImplicitPrecharge(NVMainRequest* req) {
    ncounter_t rank, bank, row, subarray, col;

    req->address.GetTranslatedAddress(&row, &col, &bank, &rank, NULL,
                                      &subarray);
    ncounter_t queueId = GetCommandQueueId(req->address);

    req->issueCycle = GetEventQueue()->GetCurrentCycle();
    commandQueues[queueId].push_back(
        reqMaker.makeImplicitPrechargeRequest(req));
    activeSubArrays.clear(req);
    activeRow.clear(req, p->ROWS);
    activeMuxedRow.clear(req, p->ROWS);
    bool idle = true;
    for (ncounter_t i = 0; i < subArrayNum; i++) {
        if (activeSubArrays[req]) {
            idle = false;
            break;
        }
    }

    if (idle) activateQueued[rank][bank] = false;
}

void MemoryController::closeRow(NVMainRequest* req) {
    ncounter_t rank, bank, row, subarray, col;

    req->address.GetTranslatedAddress(&row, &col, &bank, &rank, NULL,
                                      &subarray);
    ncounter_t queueId = GetCommandQueueId(req->address);

    if (activeSubArrays[req] && p->UsePrecharge) {
        commandQueues[queueId].push_back(reqMaker.makePrechargeRequest(
            activeRow[req], 0, bank, rank, id, subarray));
    }
}

ncounter_t getBank(NVMainRequest* req) {
    ncounter_t rank, bank, row, subarray, col;

    req->address.GetTranslatedAddress(&row, &col, &bank, &rank, NULL,
                                      &subarray);

    return bank;
}

ncounter_t getRank(NVMainRequest* req) {
    ncounter_t rank, bank, row, subarray, col;

    req->address.GetTranslatedAddress(&row, &col, &bank, &rank, NULL,
                                      &subarray);

    return rank;
}

ncounter_t getRow(NVMainRequest* req) {
    ncounter_t rank, bank, row, subarray, col;

    req->address.GetTranslatedAddress(&row, &col, &bank, &rank, NULL,
                                      &subarray);

    return row;
}

ncounter_t getCol(NVMainRequest* req) {
    ncounter_t rank, bank, row, subarray, col;

    req->address.GetTranslatedAddress(&row, &col, &bank, &rank, NULL,
                                      &subarray);

    return bank;
}

ncounter_t getSubarray(NVMainRequest* req) {
    ncounter_t rank, bank, row, subarray, col;

    req->address.GetTranslatedAddress(&row, &col, &bank, &rank, NULL,
                                      &subarray);

    return bank;
}

void MemoryController::handleActivate(NVMainRequest* req) {
    auto rank = getRank(req);
    auto bank = getBank(req);
    auto subarray = getSubarray(req);
    auto queueId = GetCommandQueueId(req->address);
    auto row = getRow(req);

    if (!bankIsActivated(req) && commandQueues[queueId].empty()) {
        enqueueActivate(req);
        std::cout << "Memory Controller - Activated bank " << bank << std::endl;
    } else if (activateQueued[rank][bank] && !rowIsActivated(req) &&
               commandQueues[queueId].empty()) {
        closeRow(req);
        enqueueActivate(req);
        std::cout << "Memory Controller switched to row " << row << std::endl;
    } else if (rowIsActivated(req)) {
        starvationCounters.increment(req);
    } else {
        throw std::runtime_error("Memory controller cannot issue request " +
                                 std::to_string(req->arrivalCycle));
    }
}

bool MemoryController::IssueMemoryCommands(NVMainRequest* req) {
    // TODO clean up this function
    if (req->type == ROWCLONE_SRC || req->type == ROWCLONE_DEST) {
        handleActivate(req);
        enqueueRequest(req);
        ScheduleCommandWake();
        return true;
    }

    if (handleCachedRequest(req)) return true;

    handleActivate(req);
    enqueueShift(req);
    if (req->flags & NVMainRequest::FLAG_LAST_REQUEST && p->UsePrecharge) {
        assert(p->ClosePage != 2);
        enqueueImplicitPrecharge(req);
    } else {
        enqueueRequest(req);
    }

    ScheduleCommandWake();
    return true;
}

void MemoryController::CycleCommandQueues() {
    if (handledRefresh == GetEventQueue()->GetCurrentCycle()) {
        return;
    }

    for (ncounter_t queueIdx = 0; queueIdx < commandQueueCount; queueIdx++) {
        ncounter_t queueId = (curQueue + queueIdx) % commandQueueCount;
        FailReason fail;

        if (!commandQueues[queueId].empty() &&
            lastIssueCycle != GetEventQueue()->GetCurrentCycle() &&
            GetChild()->IsIssuable(commandQueues[queueId].at(0), &fail)) {
            NVMainRequest* queueHead = commandQueues[queueId].at(0);

            auto row = getRow(queueHead);

            std::cout << GetEventQueue()->GetCurrentCycle()
                      << " MemoryController: Issued request type "
                      << queueHead->type << " for address 0x" << std::hex
                      << queueHead->address.GetPhysicalAddress() << std::dec
                      << " row " << row << " for queue " << queueId
                      << std::endl;

            GetChild()->IssueCommand(queueHead);

            queueHead->flags |= NVMainRequest::FLAG_ISSUED;

            if (queueHead->type == REFRESH)
                ResetRefreshQueued(queueHead->address.GetBank(),
                                   queueHead->address.GetRank());

            if (GetEventQueue()->GetCurrentCycle() != lastIssueCycle)
                lastIssueCycle = GetEventQueue()->GetCurrentCycle();

            ncycle_t cleanupCycle = GetEventQueue()->GetCurrentCycle() + 1;
            bool cleanupScheduled = GetEventQueue()->FindCallback(
                this, (CallbackPtr) &MemoryController::CleanupCallback,
                cleanupCycle, NULL, cleanupPriority);

            if (!cleanupScheduled)
                GetEventQueue()->InsertCallback(
                    this, (CallbackPtr) &MemoryController::CleanupCallback,
                    cleanupCycle, NULL, cleanupPriority);

            if (commandQueues[queueId].size() == 1) {

                if (TransactionAvailable(queueId)) {
                    ncycle_t nextWakeup =
                        GetEventQueue()->GetCurrentCycle() + 1;

                    GetEventQueue()->InsertEvent(EventCycle, this, nextWakeup,
                                                 NULL,
                                                 transactionQueuePriority);
                }
            }

            MoveCurrentQueue();

            return;
        } else if (!commandQueues[queueId].empty()) {
            NVMainRequest* queueHead = commandQueues[queueId].at(0);

            if ((GetEventQueue()->GetCurrentCycle() - queueHead->issueCycle) >
                p->DeadlockTimer) {
                ncounter_t row, col, bank, rank, channel, subarray;
                queueHead->address.GetTranslatedAddress(
                    &row, &col, &bank, &rank, &channel, &subarray);
                std::cout << "NVMain Warning: Operation could not be sent to "
                             "memory after a very long time: "
                          << std::endl;
                std::cout << "         Address: 0x" << std::hex
                          << queueHead->address.GetPhysicalAddress() << std::dec
                          << " @ Bank " << bank << ", Rank " << rank
                          << ", Channel " << channel << " Subarray " << subarray
                          << " Row " << row << " Column " << col
                          << ". Queued time: " << queueHead->arrivalCycle
                          << ". Issue time: " << queueHead->issueCycle
                          << ". Current time: "
                          << GetEventQueue()->GetCurrentCycle()
                          << ". Type: " << queueHead->type << std::endl;

                // Give the opportunity to attach a debugger here.
#ifndef NDEBUG
                raise(SIGSTOP);
#endif
                GetStats()->PrintAll(std::cerr);
                exit(1);
            }
        }
    }
}

ncounter_t MemoryController::GetCommandQueueId(NVMAddress addr) {
    ncounter_t queueId = std::numeric_limits<ncounter_t>::max();

    if (queueModel == PerRankQueues) {
        queueId = addr.GetRank();
    } else if (queueModel == PerBankQueues) {
        if (p->ScheduleScheme == 1) {
            queueId = (addr.GetBank() * p->RANKS + addr.GetRank());
        } else if (p->ScheduleScheme == 2) {
            queueId = (addr.GetRank() * p->BANKS + addr.GetBank());
        }
    } else if (queueModel == PerSubArrayQueues) {
        // TODO: ScheduleSchemes? There are 6 possible orderings now.
        queueId = (addr.GetRank() * p->BANKS * subArrayNum) +
                  (addr.GetBank() * subArrayNum) + addr.GetSubArray();
    }

    assert(queueId < commandQueueCount);

    return queueId;
}

ncycle_t MemoryController::NextIssuable(NVMainRequest*) {
    ncycle_t nextWakeup = std::numeric_limits<ncycle_t>::max();

    for (ncounter_t rankIdx = 0; rankIdx < p->RANKS; rankIdx++) {
        for (ncounter_t bankIdx = 0; bankIdx < p->BANKS; bankIdx++) {
            ncounter_t queueIdx =
                GetCommandQueueId(NVMAddress(0, 0, bankIdx, rankIdx, 0, 0));
            if (NeedRefresh(bankIdx, rankIdx) &&
                IsRefreshBankQueueEmpty(bankIdx, rankIdx)) {
                if (lastIssueCycle != GetEventQueue()->GetCurrentCycle())
                    HandleRefresh();
                else nextWakeup = GetEventQueue()->GetCurrentCycle() + 1;
            }

            if (commandQueues[queueIdx].empty()) continue;

            NVMainRequest* queueHead = commandQueues[queueIdx].at(0);

            nextWakeup = MIN(nextWakeup, GetChild()->NextIssuable(queueHead));
        }
    }

    if (nextWakeup <= GetEventQueue()->GetCurrentCycle())
        nextWakeup = GetEventQueue()->GetCurrentCycle() + 1;

    return nextWakeup;
}

bool MemoryController::RankQueueEmpty(const ncounter_t& rankId) {
    bool rv = true;

    for (ncounter_t i = 0; i < p->BANKS; i++) {
        ncounter_t queueId =
            GetCommandQueueId(NVMAddress(0, 0, i, rankId, 0, 0));
        if (commandQueues[queueId].empty() == false) {
            rv = false;
            break;
        }
    }

    return rv;
}

bool MemoryController::EffectivelyEmpty(const ncounter_t& queueId) {
    assert(queueId < commandQueueCount);

    bool effectivelyEmpty = (commandQueues[queueId].size() == 1) &&
                            (WasIssued(commandQueues[queueId].at(0)));

    return (commandQueues[queueId].empty() || effectivelyEmpty);
}

void MemoryController::MoveCurrentQueue() {
    if (p->ScheduleScheme != 0) {
        curQueue++;
        if (curQueue > commandQueueCount) {
            curQueue = 0;
        }
    }
}

void MemoryController::CalculateStats() {
    ncycle_t syncCycles = GetEventQueue()->GetCurrentCycle() - lastCommandWake;
    GetChild()->Cycle(syncCycles);

    simulation_cycles = GetEventQueue()->GetCurrentCycle();

    GetChild()->CalculateStats();
    GetDecoder()->CalculateStats();
}

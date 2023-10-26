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
#include "Logging/Logging.h"
#include "src/EventQueue.h"
#include "src/Interconnect.h"
#include "src/Rank.h"
#include "src/SubArray.h"

#include <algorithm>
#include <cassert>
#include <csignal>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>

using namespace NVM;
using namespace NVM::Logging;

/**************************PUBLIC FUNCTIONS*****************************/

MemoryController::MemoryController() : reqFinder(this) {
    transactionQueues = NULL;
    transactionQueueCount = 0;

    lastCommandWake = 0;
    wakeupCount = 0;
    lastIssueCycle = 0;

    starvationThreshold = 4;

    curQueue = 0;
}

bool MemoryController::RequestComplete(NVMainRequest* request) {
    if (request->owner == this) {
        delete request;
        return true;
    }
    return GetParent()->RequestComplete(request);
}

bool MemoryController::IsIssuable(NVMainRequest*, FailReason*) { return true; }

ncycle_t MemoryController::NextIssuable(NVMainRequest*) {
    if (refreshHandler.canRefresh() &&
        lastIssueCycle != GetEventQueue()->GetCurrentCycle())
        refreshHandler.HandleRefresh();

    ncycle_t nextWakeup = std::numeric_limits<ncycle_t>::max();
    for (auto* head : commandQueues) {
        nextWakeup = std::min(nextWakeup, GetChild()->NextIssuable(head));
    }
    return std::max(nextWakeup, GetEventQueue()->GetCurrentCycle() + 1);
}

void MemoryController::RegisterStats() {
    AddStat(simulation_cycles);
    AddStat(wakeupCount);
}

void MemoryController::CalculateStats() {
    ncycle_t syncCycles = GetEventQueue()->GetCurrentCycle() - lastCommandWake;
    GetChild()->Cycle(syncCycles);

    simulation_cycles = GetEventQueue()->GetCurrentCycle();

    GetChild()->CalculateStats();
    GetDecoder()->CalculateStats();
}

void MemoryController::Cycle(ncycle_t) {
    std::vector<size_t> indices(commandQueues.size());
    std::iota(indices.begin(), indices.end(), 0);
    if (std::any_of(indices.begin(), indices.end(), [&](auto i) {
            return commandQueues.effectivelyEmpty(i) && TransactionAvailable(i);
        })) {
        queueCycle(GetEventQueue()->GetCurrentCycle() + 1);
    }
}

void MemoryController::SetConfig(Config* conf, bool createChildren) {
    Params* params = new Params();
    params->SetParams(conf);
    SetParams(params);

    commandQueues.setConfig(conf);

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

        memory->SetConfig(conf, createChildren);
        memory->RegisterStats();

        SetMappingScheme();
    }

    Logging::log() << LogLevel::STAT << StatName() << " capacity is "
                   << ((p->ROWS * p->COLS * p->tBURST * p->RATE * p->BusWidth *
                        p->BANKS * p->RANKS) /
                       (8 * 1024))
                   << " KB." << '\n';

    starvationCounters = SubArrayCounter(p, conf);
    activeSubArrays = SubArrayCounter(p, conf);
    activeRow = SubArrayCounter(p, conf, p->ROWS);
    activeMuxedRow = SubArrayCounter(p, conf, p->ROWS);
    bankActivated = BankCounter(p);
    refreshHandler = RefreshHandler(this, p, GetEventQueue());

    if (p->PrintConfig) conf->Print();

    SetDebugName("MemoryController", conf);
}

void MemoryController::SetID(unsigned int id) { this->id = id; }

unsigned int MemoryController::GetID() { return this->id; }

/*****************************************************************/

void MemoryController::InitQueues(unsigned int numQueues) {
    if (transactionQueues != NULL) delete[] transactionQueues;

    transactionQueues = new NVMTransactionQueue[numQueues];
    transactionQueueCount = numQueues;
}

void MemoryController::Prequeue(ncounter_t queueNum, NVMainRequest* request) {
    assert(queueNum < transactionQueueCount);

    transactionQueues[queueNum].push_front(request);
}

void MemoryController::queueCycle(ncycle_t cycle) {
    if (GetEventQueue()->FindEvent(EventCycle, this, NULL, cycle) == NULL) {
        GetEventQueue()->InsertEvent(EventCycle, this, cycle, NULL,
                                     transactionQueuePriority);
    }
}

void MemoryController::Enqueue(ncounter_t queueNum, NVMainRequest* request) {
    ncounter_t channel, rank, bank, row, col, subarray;

    GetDecoder()->Translate(request->address.GetPhysicalAddress(), &row, &col,
                            &bank, &rank, &channel, &subarray);
    channel = request->address.GetChannel();
    request->address.SetTranslatedAddress(row, col, bank, rank, channel,
                                          subarray);

    transactionQueues[queueNum].push_back(request);

    if (commandQueues.effectivelyEmpty(request->address)) {
        queueCycle(GetEventQueue()->GetCurrentCycle());
    }
}

bool MemoryController::TransactionAvailable(ncounter_t queueId) {
    for (int i = 0; i < transactionQueueCount; i++) {
        auto transaction = std::find_if(
            transactionQueues[i].begin(), transactionQueues[i].end(),
            [&](NVMainRequest* req) {
                return commandQueues.getQueueIndex(req->address) == queueId;
            });
        if (transaction != transactionQueues[i].end()) return true;
    }
    return false;
}

void MemoryController::ScheduleCommandWake() {
    queueCallback(NextIssuable(NULL),
                  (CallbackPtr) &MemoryController::CommandQueueCallback,
                  commandQueuePriority);
}

void MemoryController::CommandQueueCallback(void*) {
    ncycle_t realSteps = GetEventQueue()->GetCurrentCycle() - lastCommandWake;
    lastCommandWake = GetEventQueue()->GetCurrentCycle();
    wakeupCount++;
    ScheduleCommandWake();
    CycleCommandQueues();
    GetChild()->Cycle(realSteps);
}

void MemoryController::CleanupCallback(void*) { commandQueues.removeIssued(); }

void MemoryController::SetMappingScheme() {
    GetDecoder()->GetTranslationMethod()->SetAddressMappingScheme(
        p->AddressMappingScheme);
}

void MemoryController::RefreshCallback(void* data) {
    NVMainRequest* request = reinterpret_cast<NVMainRequest*>(data);

    ncycle_t realSteps = GetEventQueue()->GetCurrentCycle() - lastCommandWake;
    lastCommandWake = GetEventQueue()->GetCurrentCycle();
    wakeupCount++;

    refreshHandler.ProcessRefreshPulse(request);
    refreshHandler.HandleRefresh();

    GetChild()->Cycle(realSteps);
}

bool MemoryController::IsLastRequest(
    std::list<NVMainRequest*>& transactionQueue, NVMainRequest* request) {
    if (p->ClosePage == 0) return false;

    ncounter_t mRank, mBank, mRow, mSubArray;
    request->address.GetTranslatedAddress(&mRow, NULL, &mBank, &mRank, NULL,
                                          &mSubArray);
    auto reqInQueue =
        std::find_if(transactionQueue.begin(), transactionQueue.end(),
                     [&](NVMainRequest* req) {
                         ncounter_t rank, bank, row, subarray;
                         req->address.GetTranslatedAddress(
                             &row, NULL, &bank, &rank, NULL, &subarray);

                         return (rank == mRank && bank == mBank &&
                                 row == mRow && subarray == mSubArray);
                     });

    return reqInQueue == transactionQueue.end();
}

bool MemoryController::bankIsActivated(NVMainRequest* req) {
    return bankActivated[req];
}

bool MemoryController::handleCachedRequest(NVMainRequest* req) {
    NVMainRequest* cachedRequest = reqMaker.makeCachedRequest(req);

    if (GetChild()->IsIssuable(cachedRequest, nullptr)) {
        if (!bankActivated[req] || !rowIsActivated(req)) {
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

    return activeSubArrays[req] && activeRow[req] == row &&
           activeMuxedRow[req] == muxLevel;
}

void MemoryController::enqueueActivate(NVMainRequest* req) {
    ncounter_t rank, bank, row, subarray, col;

    req->address.GetTranslatedAddress(&row, &col, &bank, &rank, NULL,
                                      &subarray);
    NVMainRequest* actRequest = reqMaker.makeActivateRequest(req, this);
    SubArray* writingArray = FindChild(req, SubArray);
    actRequest->flags |= (writingArray != NULL && writingArray->IsWriting())
                             ? NVMainRequest::FLAG_PRIORITY
                             : 0;
    enqueueRequest(actRequest);

    ncounter_t muxLevel = static_cast<ncounter_t>(col / p->RBSize);
    bankActivated[req] = true;
    activeSubArrays.increment(req);
    activeRow[req] = row;
    activeMuxedRow[req] = muxLevel;
    starvationCounters.clear(req);
}

void MemoryController::enqueueShift(NVMainRequest* req) {
    if (!p->MemIsRTM) return;
    SubArray* writingArray = FindChild(req, SubArray);
    NVMainRequest* shiftRequest = reqMaker.makeShiftRequest(req, this);
    if (writingArray != NULL && writingArray->IsWriting())
        shiftRequest->flags |= NVMainRequest::FLAG_PRIORITY;
    enqueueRequest(shiftRequest);
}

void MemoryController::enqueueRequest(NVMainRequest* req) {
    req->issueCycle = GetEventQueue()->GetCurrentCycle();
    commandQueues.enqueue(req);
}

void MemoryController::enqueueImplicitPrecharge(NVMainRequest* req) {
    req->issueCycle = GetEventQueue()->GetCurrentCycle();
    enqueueRequest(reqMaker.convertToImplicitPrecharge(req));
    activeSubArrays.clear(req);
    activeRow.clear(req, p->ROWS);
    activeMuxedRow.clear(req, p->ROWS);

    auto rank = req->address.GetRank(), bank = req->address.GetBank();
    auto idle = std::all_of(activeSubArrays.begin(rank, bank),
                            activeSubArrays.end(rank, bank),
                            [](auto val) { return val == 0; });
    if (idle) bankActivated.clear(req);
}

void MemoryController::closeRow(NVMainRequest* req) {
    if (activeSubArrays[req] && p->UsePrecharge) {
        auto preReq = reqMaker.makePrechargeRequest(req, this);
        preReq->address.setRow(activeRow[req]);
        enqueueRequest(preReq);
    }
}

void MemoryController::handleActivate(NVMainRequest* req) {
    if (!bankIsActivated(req)) {
        enqueueActivate(req);
        Logging::log() << LogLevel::DEBUG
                       << "Memory Controller - Activated bank "
                       << req->address.GetRow() << '\n';
    } else if (bankActivated[req] && !rowIsActivated(req)) {
        closeRow(req);
        enqueueActivate(req);
        Logging::log() << LogLevel::DEBUG
                       << "Memory Controller switched to row "
                       << req->address.GetRow() << '\n';
    } else if (rowIsActivated(req)) {
        starvationCounters.increment(req);
    } else {
        throw std::runtime_error("Memory controller cannot issue request " +
                                 std::to_string(req->arrivalCycle));
    }
}

bool MemoryController::IssueMemoryCommands(NVMainRequest* req) {
    log() << LogLevel::DEBUG << "Handling request " << req->arrivalCycle
          << '\n';

    if (req->address.IsTranslated()) {
        log() << LogLevel::DEBUG << "Address is translated...\n";
        log() << std::hex << req->address.GetPhysicalAddress() << '\n';
        log() << "Row: " << req->address.GetRow() << '\n';
        log() << "Col: " << req->address.GetCol() << '\n';
        log() << "Rank: " << req->address.GetRank() << '\n';
        log() << "Bank: " << req->address.GetBank() << '\n';
        log() << "Channel: " << req->address.GetChannel() << '\n';
    }

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

bool MemoryController::issuedLastTransaction(NVMAddress addr) {
    return commandQueues.effectivelyEmpty(addr) &&
           TransactionAvailable(commandQueues.getQueueIndex(addr));
}

void MemoryController::queueCallback(ncycle_t cycle, CallbackPtr callback,
                                     int priority) {
    if (!GetEventQueue()->FindCallback(this, callback, cycle, NULL, priority))
        GetEventQueue()->InsertCallback(this, callback, cycle, NULL, priority);
}

void MemoryController::CycleCommandQueues() {
    if (refreshHandler.getLastRefresh() == GetEventQueue()->GetCurrentCycle() ||
        lastIssueCycle == GetEventQueue()->GetCurrentCycle()) {
        return;
    }
    auto reqToIssueIt =
        std::find_if(commandQueues.begin(curQueue), commandQueues.end(curQueue),
                     [&](NVMainRequest* req) {
                         return GetChild()->IsIssuable(req, nullptr);
                     });
    if (reqToIssueIt == commandQueues.end(curQueue)) {
        commandQueues.checkForDeadlock(GetEventQueue()->GetCurrentCycle());
        return;
    }

    auto req = *reqToIssueIt;

    auto row = req->address.GetRow();
    Logging::log() << LogLevel::DEBUG << GetEventQueue()->GetCurrentCycle()
                   << " MemoryController: Issued request type " << req->type
                   << " for address 0x" << std::hex
                   << req->address.GetPhysicalAddress() << std::dec << " row "
                   << row << '\n';

    GetChild()->IssueCommand(req);
    req->flags |= NVMainRequest::FLAG_ISSUED;

    if (req->type == REFRESH)
        refreshHandler.ResetRefreshQueued(req->address.GetBank(),
                                          req->address.GetRank());

    lastIssueCycle = GetEventQueue()->GetCurrentCycle();
    queueCallback(lastIssueCycle + 1,
                  (CallbackPtr) &MemoryController::CleanupCallback,
                  cleanupPriority);
    if (issuedLastTransaction(req->address)) queueCycle(lastIssueCycle + 1);

    if (p->ScheduleScheme != 0)
        curQueue = (curQueue + 1) % commandQueues.size();
}

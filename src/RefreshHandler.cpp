#include "src/RefreshHandler.h"

#include "include/NVMAddress.h"
#include "src/MemoryController.h"

using namespace NVM;

RefreshHandler::RefreshHandler() {}

RefreshHandler::RefreshHandler(MemoryController* parent, Params* params,
                               EventQueue* parentQueue) :
    parent(parent),
    params(params),
    parentQueue(parentQueue),
    delayedRefreshCounter(params),
    needsRefreshCounter(params),
    queued(params) {}

bool RefreshHandler::NeedRefresh(const ncounter_t bank, const uint64_t rank) {
    bool rv = false;

    if (params->UseRefresh)
        if (delayedRefreshCounter[NVMAddress(
                0, 0, bank / params->BanksPerRefresh, rank, 0, 0)] >=
            params->DelayedRefreshThreshold)
            rv = true;

    return rv;
}

void RefreshHandler::SetRefresh(const ncounter_t bank, const uint64_t rank) {
    ncounter_t bankHead =
        (bank / params->BanksPerRefresh) * params->BanksPerRefresh;

    for (ncounter_t i = 0; i < params->BanksPerRefresh; i++)
        needsRefreshCounter[NVMAddress(0, 0, bankHead + i, rank, 0, 0)] = true;
}

void RefreshHandler::ResetRefresh(const ncounter_t bank, const uint64_t rank) {
    ncounter_t bankHead =
        (bank / params->BanksPerRefresh) * params->BanksPerRefresh;

    for (ncounter_t i = 0; i < params->BanksPerRefresh; i++)
        needsRefreshCounter[NVMAddress(0, 0, bankHead + i, rank, 0, 0)] = false;
}

void RefreshHandler::ResetRefreshQueued(const ncounter_t bank,
                                        const ncounter_t rank) {
    ncounter_t bankHead =
        (bank / params->BanksPerRefresh) * params->BanksPerRefresh;

    for (ncounter_t i = 0; i < params->BanksPerRefresh; i++) {
        // assert(refreshQueued[rank][bankHead + i]);
        queued[NVMAddress(0, 0, bankHead + i, rank, 0, 0)] = false;
    }
}

void RefreshHandler::IncrementRefreshCounter(const ncounter_t bank,
                                             const uint64_t rank) {
    ncounter_t bankGroupID = bank / params->BanksPerRefresh;

    delayedRefreshCounter[NVMAddress(0, 0, bankGroupID, rank, 0, 0)]++;
}

void RefreshHandler::DecrementRefreshCounter(const ncounter_t bank,
                                             const uint64_t rank) {
    ncounter_t bankGroupID = bank / params->BanksPerRefresh;

    delayedRefreshCounter[NVMAddress(0, 0, bankGroupID, rank, 0, 0)]--;
}

bool RefreshHandler::HandleRefresh() {
    for (ncounter_t rankIdx = 0; rankIdx < params->RANKS; rankIdx++) {
        ncounter_t i = (parent->nextRefreshRank + rankIdx) % params->RANKS;

        for (ncounter_t bankIdx = 0; bankIdx < parent->m_refreshBankNum;
             bankIdx++) {
            ncounter_t j =
                (parent->nextRefreshBank + bankIdx * params->BanksPerRefresh) %
                params->BANKS;
            FailReason fail;

            if (NeedRefresh(j, i)) {
                NVMainRequest* cmdRefresh = parent->reqMaker.makeRefreshRequest(
                    0, 0, j, i, parent->id, 0);

                if (params->UsePrecharge) {
                    for (ncounter_t tmpBank = 0;
                         tmpBank < params->BanksPerRefresh; tmpBank++) {
                        ncounter_t refBank = (tmpBank + j) % params->BANKS;

                        if (parent->bankActivated[NVMAddress(0, 0, refBank, i,
                                                             0, 0)] == true) {
                            NVMainRequest* cmdRefPre =
                                parent->reqMaker.makePrechargeAllRequest(
                                    0, 0, refBank, i, parent->id, 0);

                            parent->commandQueues.enqueue(cmdRefPre);
                            parent->activeSubArrays.clear(i, refBank);
                            parent->activeRow.clear(i, refBank, params->ROWS);
                            parent->activeMuxedRow.clear(i, refBank,
                                                         params->ROWS);
                            parent->bankActivated[NVMAddress(0, 0, refBank, i,
                                                             0, 0)] = false;
                        }
                    }
                }

                cmdRefresh->issueCycle = parentQueue->GetCurrentCycle();
                parent->commandQueues.enqueue(cmdRefresh);

                for (ncounter_t tmpBank = 0; tmpBank < params->BanksPerRefresh;
                     tmpBank++) {
                    ncounter_t refBank = (tmpBank + j) % params->BANKS;

                    queued[NVMAddress(0, 0, refBank, i, 0, 0)] = true;
                }

                DecrementRefreshCounter(j, i);

                if (!NeedRefresh(j, i)) ResetRefresh(j, i);

                parent->nextRefreshBank += params->BanksPerRefresh;
                if (parent->nextRefreshBank >= params->BANKS) {
                    parent->nextRefreshBank = 0;
                    parent->nextRefreshRank++;

                    if (parent->nextRefreshRank == params->RANKS)
                        parent->nextRefreshRank = 0;
                }

                parent->handledRefresh = parentQueue->GetCurrentCycle();

                parent->ScheduleCommandWake();

                return true;
            }
        }
    }
    return false;
}

void RefreshHandler::ProcessRefreshPulse(NVMainRequest* refresh) {
    // assert(refresh->type == REFRESH);

    ncounter_t rank, bank;
    refresh->address.GetTranslatedAddress(NULL, NULL, &bank, &rank, NULL, NULL);

    IncrementRefreshCounter(bank, rank);

    if (NeedRefresh(bank, rank)) SetRefresh(bank, rank);

    parentQueue->InsertCallback(
        parent, (CallbackPtr) &MemoryController::RefreshCallback,
        parentQueue->GetCurrentCycle() + parent->m_tREFI,
        reinterpret_cast<void*>(refresh), parent->refreshPriority);
}

bool RefreshHandler::IsRefreshBankQueueEmpty(const ncounter_t bank,
                                             const uint64_t rank) {
    ncounter_t bankHead =
        (bank / params->BanksPerRefresh) * params->BanksPerRefresh;

    for (ncounter_t i = 0; i < params->BanksPerRefresh; i++) {
        ncounter_t queueId = parent->GetCommandQueueId(
            NVMAddress(0, 0, bankHead + i, rank, 0, 0));
        if (!parent->commandQueues.effectivelyEmpty(queueId)) {
            return false;
        }
    }

    return true;
}

bool RefreshHandler::refreshQueued(NVMainRequest* req) { return queued[req]; }

bool RefreshHandler::bankNeedsRefresh(NVMainRequest* req) {
    return needsRefreshCounter[req];
}

bool RefreshHandler::needsRefresh(NVMAddress address) {
    return needsRefreshCounter[address];
}

bool RefreshHandler::canRefresh(NVMAddress address) {
    ncounter_t bankHead =
        (address.GetBank() / params->BanksPerRefresh) * params->BanksPerRefresh;

    for (ncounter_t i = 0; i < params->BanksPerRefresh; i++) {
        auto bankAddr = NVMAddress(0, 0, bankHead + i, address.GetRank(), 0, 0);
        if (!parent->commandQueues.effectivelyEmpty(bankAddr)) {
            return false;
        }
    }

    return needsRefreshCounter[address];
}

bool RefreshHandler::canRefresh() {
    for (int i = 0; i < params->RANKS; i++)
        for (int j = 0; j < params->BANKS; j += params->BanksPerRefresh)
            if (canRefresh(NVMAddress(0, 0, j, i, 0, 0))) return true;
    return false;
}
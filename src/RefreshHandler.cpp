#include "src/RefreshHandler.h"

#include "src/MemoryController.h"

using namespace NVM;

/*
delayedRefreshCounter
bankNeedRefresh
refreshQueued
nextRefreshRank
m_refreshBankNum
*/

RefreshHandler::RefreshHandler() {}

RefreshHandler::RefreshHandler(MemoryController* parent, Params* params,
                               EventQueue* parentQueue) :
    parent(parent),
    params(params),
    parentQueue(parentQueue) {}

bool RefreshHandler::NeedRefresh(const ncounter_t bank, const uint64_t rank) {
    bool rv = false;

    if (params->UseRefresh)
        if (parent
                ->delayedRefreshCounter[rank][bank / params->BanksPerRefresh] >=
            params->DelayedRefreshThreshold)
            rv = true;

    return rv;
}

void RefreshHandler::SetRefresh(const ncounter_t bank, const uint64_t rank) {
    ncounter_t bankHead =
        (bank / params->BanksPerRefresh) * params->BanksPerRefresh;

    for (ncounter_t i = 0; i < params->BanksPerRefresh; i++)
        parent->bankNeedRefresh[rank][bankHead + i] = true;
}

void RefreshHandler::ResetRefresh(const ncounter_t bank, const uint64_t rank) {
    ncounter_t bankHead =
        (bank / params->BanksPerRefresh) * params->BanksPerRefresh;

    for (ncounter_t i = 0; i < params->BanksPerRefresh; i++)
        parent->bankNeedRefresh[rank][bankHead + i] = false;
}

void RefreshHandler::ResetRefreshQueued(const ncounter_t bank,
                                        const ncounter_t rank) {
    ncounter_t bankHead =
        (bank / params->BanksPerRefresh) * params->BanksPerRefresh;

    for (ncounter_t i = 0; i < params->BanksPerRefresh; i++) {
        // assert(refreshQueued[rank][bankHead + i]);
        parent->refreshQueued[rank][bankHead + i] = false;
    }
}

void RefreshHandler::IncrementRefreshCounter(const ncounter_t bank,
                                             const uint64_t rank) {
    ncounter_t bankGroupID = bank / params->BanksPerRefresh;

    parent->delayedRefreshCounter[rank][bankGroupID]++;
}

void RefreshHandler::DecrementRefreshCounter(const ncounter_t bank,
                                             const uint64_t rank) {
    ncounter_t bankGroupID = bank / params->BanksPerRefresh;

    parent->delayedRefreshCounter[rank][bankGroupID]--;
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

                    parent->refreshQueued[i][refBank] = true;
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
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
 * (e.g., PCRAM). The source code is free and you can redistribute and/or
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

#ifndef __MEMORYCONTROLLER_H__
#define __MEMORYCONTROLLER_H__

#include "include/NVMainRequest.h"
#include "src/AddressTranslator.h"
#include "src/Config.h"
#include "src/Interconnect.h"
#include "src/NVMObject.h"
#include "src/SubArrayCounter.h"

#include <deque>
#include <iostream>
#include <list>
#include <string>
#include <vector>

namespace NVM {

class SchedulingPredicate {
    public:
    SchedulingPredicate() {}

    ~SchedulingPredicate() {}

    virtual bool operator()(NVMainRequest* /*request*/) { return true; }
};

class MemoryController : public NVMObject {
    public:
    // Public Functions
    MemoryController();
    ~MemoryController();
    virtual bool RequestComplete(NVMainRequest* request);
    virtual bool IsIssuable(NVMainRequest* request, FailReason* fail);
    ncycle_t NextIssuable(NVMainRequest* request);
    virtual void RegisterStats();
    virtual void CalculateStats();
    virtual void Cycle(ncycle_t steps);
    virtual void SetConfig(Config* conf, bool createChildren = true);
    void SetID(unsigned int id);

    protected:
    ncounter_t psInterval;
    std::list<NVMainRequest*>* transactionQueues;
    std::deque<NVMainRequest*>* commandQueues;
    bool** activateQueued;
    SubArrayCounter activeRow;
    SubArrayCounter starvationCounters;
    ncounter_t starvationThreshold;

    void InitQueues(unsigned int numQueues);
    ncounter_t GetCommandQueueId(NVMAddress addr);
    void Prequeue(ncounter_t queueNum, NVMainRequest* request);
    void Enqueue(ncounter_t queueNum, NVMainRequest* request);
    NVMainRequest* MakeActivateRequest(NVMainRequest* triggerRequest);
    NVMainRequest* MakePrechargeRequest(NVMainRequest* triggerRequest);
    bool FindStarvedRequest(std::list<NVMainRequest*>& transactionQueue,
                            NVMainRequest** starvedRequest);
    bool FindCachedAddress(std::list<NVMainRequest*>& transactionQueue,
                           NVMainRequest** accessibleRequest);
    bool FindRowBufferHit(std::list<NVMainRequest*>& transactionQueue,
                          NVMainRequest** hitRequest);
    bool FindRTMRowBufferHit(std::list<NVMainRequest*>& transactionQueue,
                             NVMainRequest** hitRequest);
    bool FindWriteStalledRead(std::list<NVMainRequest*>& transactionQueue,
                              NVMainRequest** hitRequest);
    bool FindOldestReadyRequest(std::list<NVMainRequest*>& transactionQueue,
                                NVMainRequest** oldestRequest);
    bool FindClosedBankRequest(std::list<NVMainRequest*>& transactionQueue,
                               NVMainRequest** closedRequest);
    bool IssueMemoryCommands(NVMainRequest* req);
    void CycleCommandQueues();
    bool FindStarvedRequest(std::list<NVMainRequest*>& transactionQueue,
                            NVMainRequest** starvedRequest,
                            NVM::SchedulingPredicate& p);
    bool FindRowBufferHit(std::list<NVMainRequest*>& transactionQueue,
                          NVMainRequest** hitRequest,
                          NVM::SchedulingPredicate& p);
    bool FindOldestReadyRequest(std::list<NVMainRequest*>& transactionQueue,
                                NVMainRequest** oldestRequest,
                                NVM::SchedulingPredicate& p);
    bool FindClosedBankRequest(std::list<NVMainRequest*>& transactionQueue,
                               NVMainRequest** closedRequest,
                               NVM::SchedulingPredicate& p);
    /* basically, it increment the delayedRefreshCounter and generate the next
     * refresh pulse */
    void ProcessRefreshPulse(NVMainRequest*);

    private:
    enum QueueModel { PerRankQueues, PerBankQueues, PerSubArrayQueues };

    // Private Vars
    SubArrayCounter activeSubArrays;
    ncycle_t lastCommandWake;
    ncounter_t wakeupCount;
    ncycle_t lastIssueCycle;
    ncounter_t commandQueueCount;
    ncounter_t transactionQueueCount;
    QueueModel queueModel;
    bool** refreshQueued;
    SubArrayCounter activeMuxedRow;
    ncounter_t subArrayNum;
    bool* rankPowerDown;
    ncounter_t id;
    ncounter_t simulation_cycles; // Stats
    ncounter_t**
        delayedRefreshCounter; // Record how many refresh should be handled
    bool** bankNeedRefresh;    // indicate whether the bank need to be refreshed
                               // immediately
    ncycle_t m_tREFI;          // indicate how long a bank should be refreshed
    ncounter_t
        m_refreshBankNum; // indicate the number of bank groups for refresh
    ncounter_t curQueue;  // curQueue records the starting index for queue
                          // round-robin level scheduling
    ncycle_t handledRefresh;
    ncounter_t nextRefreshRank, nextRefreshBank; // next Refresh rank and bank
    /*
     *  If the transaction queue has higher priority, it is possible for a
     *  transaction to be inserted into the command queue AND issued in the
     *  same clock cycle.
     *
     *  By default, the transaction queue has *lower* priority to more closely
     *  model an execution driven order.
     */
    const int transactionQueuePriority = 30;
    const int commandQueuePriority = 40;
    const int refreshPriority = 20;
    const int cleanupPriority = -10;

    void CommandQueueCallback(void* data);
    void CleanupCallback(void* data);
    void RefreshCallback(void* data);
    void SetMappingScheme();
    unsigned int GetID();
    NVMainRequest* MakeCachedRequest(NVMainRequest* triggerRequest);
    bool TransactionAvailable(ncounter_t queueId);
    void ScheduleCommandWake();
    NVMainRequest* MakeShiftRequest(NVMainRequest* triggerRequest);
    NVMainRequest* MakeImplicitPrechargeRequest(NVMainRequest* triggerRequest);
    NVMainRequest* MakePrechargeRequest(const ncounter_t, const ncounter_t,
                                        const ncounter_t, const ncounter_t,
                                        const ncounter_t);
    NVMainRequest* MakePrechargeAllRequest(const ncounter_t, const ncounter_t,
                                           const ncounter_t, const ncounter_t,
                                           const ncounter_t);
    NVMainRequest* MakeRefreshRequest(const ncounter_t, const ncounter_t,
                                      const ncounter_t, const ncounter_t,
                                      const ncounter_t);
    NVMainRequest* MakePowerdownRequest(OpType pdOp, const ncounter_t rank);
    NVMainRequest* MakePowerupRequest(const ncounter_t rank);
    bool FindCachedAddress(std::list<NVMainRequest*>& transactionQueue,
                           NVMainRequest** accessibleRequest,
                           NVM::SchedulingPredicate& p);
    bool FindRTMRowBufferHit(std::list<NVMainRequest*>& transactionQueue,
                             NVMainRequest** hitRequest,
                             NVM::SchedulingPredicate& p);
    bool FindWriteStalledRead(std::list<NVMainRequest*>& transactionQueue,
                              NVMainRequest** hitRequest,
                              NVM::SchedulingPredicate& p);

    /*
     * Tells whether no other request has the row buffer hit in the transaction
     * queue
     */
    virtual bool IsLastRequest(std::list<NVMainRequest*>& transactionQueue,
                               NVMainRequest* request);

    /*
     * Increments curQueue
     */
    void MoveCurrentQueue();

    /*
     * Returns true if the delayed refresh in the corresponding bank reach the
     * threshold
     */
    bool NeedRefresh(const ncounter_t, const ncounter_t);

    /*
     * Returns true if ALL command queues in the bank group are empty
     */
    bool IsRefreshBankQueueEmpty(const ncounter_t, const ncounter_t);

    /*
     * Sets the refresh flag for a given bank group
     */
    void SetRefresh(const ncounter_t, const ncounter_t);

    /*
     * Resets the refresh flag for a given bank group
     */
    void ResetRefresh(const ncounter_t, const ncounter_t);

    /*
     * Resets the refresh queued flag for a given bank group
     */
    void ResetRefreshQueued(const ncounter_t bank, const ncounter_t rank);

    /*
     * Increments the delayedRefreshCounter in a given bank group
     */
    void IncrementRefreshCounter(const ncounter_t, const ncounter_t);

    /*
     * Decrements the delayedRefreshCounter in a given bank group
     */
    void DecrementRefreshCounter(const ncounter_t, const ncounter_t);

    /*
     * Issues REFRESH command if necessary
     */
    virtual bool HandleRefresh();

    /*
     * Checks whether any all command queues in the rank are empty
     */
    bool RankQueueEmpty(const ncounter_t&);

    void PowerDown(const ncounter_t&);

    void PowerUp(const ncounter_t&);

    /*
     * Checks if a command queue is empty or will be cleaned up.
     */
    bool EffectivelyEmpty(const ncounter_t&);

    class DummyPredicate : public SchedulingPredicate {
        public:
        bool operator()(NVMainRequest* request);
    };

    bool bankIsActivated(NVMainRequest* req);
    bool rowIsActivated(NVMainRequest* req);
    void handleActivate(NVMainRequest* req);
    void enqueueActivate(NVMainRequest* req);
    void enqueueShift(NVMainRequest* req);
    void enqueueRequest(NVMainRequest* req);
    void enqueueImplicitPrecharge(NVMainRequest* req);
    void closeRow(NVMainRequest* req);
    bool handleCachedRequest(NVMainRequest* req);
};

}; // namespace NVM

#endif

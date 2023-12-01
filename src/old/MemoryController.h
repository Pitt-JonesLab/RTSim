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

#include "src/old/AddressTranslator.h"
#include "src/old/BankCounter.h"
#include "src/old/CommandQueues.h"
#include "src/old/Config.h"
#include "src/old/Interconnect.h"
#include "src/old/NVMainRequest.h"
#include "src/old/NVMObject.h"
#include "src/old/RefreshHandler.h"
#include "src/old/RequestFinder.h"
#include "src/old/RequestMaker.h"
#include "src/old/SubArrayCounter.h"

#include <deque>
#include <functional>
#include <iostream>
#include <list>
#include <string>
#include <vector>


namespace NVM {

class MemoryController : public NVMObject {
    public:
    // Public Functions
    MemoryController();

    ~MemoryController() {}

    virtual bool RequestComplete(NVMainRequest* request);
    virtual bool IsIssuable(NVMainRequest* request, FailReason* fail);
    ncycle_t NextIssuable(NVMainRequest* request);
    virtual void RegisterStats();
    virtual void CalculateStats();
    virtual void Cycle(ncycle_t steps);
    virtual void SetConfig(Config* conf, bool createChildren = true);
    void SetID(unsigned int id);
    unsigned int GetID();

    protected:
    ncounter_t psInterval;
    std::list<NVMainRequest*>* transactionQueues;
    CommandQueues commandQueues;
    BankCounter bankActivated;
    SubArrayCounter activeRow;
    SubArrayCounter starvationCounters;
    ncounter_t starvationThreshold;
    RequestMaker reqMaker;
    RequestFinder reqFinder;
    RefreshHandler refreshHandler;

    void InitQueues(unsigned int numQueues);

    void Prequeue(ncounter_t queueNum, NVMainRequest* request);
    void Enqueue(ncounter_t queueNum, NVMainRequest* request);
    bool IssueMemoryCommands(NVMainRequest* req);
    void CycleCommandQueues();

    bool issuedLastTransaction(NVMAddress addr);

    void queueCallback(ncycle_t cycle, CallbackPtr callback, int priority);
    void queueCycle(ncycle_t cycle);

    void enqueueRequest(NVMainRequest* req);

    private:
    SubArrayCounter activeSubArrays;
    ncycle_t lastCommandWake;
    ncounter_t wakeupCount;
    ncycle_t lastIssueCycle;
    ncounter_t transactionQueueCount;
    SubArrayCounter activeMuxedRow;
    ncounter_t id;
    ncounter_t simulation_cycles; // Stats
    ncounter_t curQueue; // curQueue records the starting index for queue
                         // round-robin level scheduling

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
    bool TransactionAvailable(ncounter_t queueId);
    void ScheduleCommandWake();

    /*
     * Tells whether no other request has the row buffer hit in the transaction
     * queue
     */
    virtual bool IsLastRequest(std::list<NVMainRequest*>& transactionQueue,
                               NVMainRequest* request);

    bool bankIsActivated(NVMainRequest* req);
    bool rowIsActivated(NVMainRequest* req);
    void handleActivate(NVMainRequest* req);
    void enqueueActivate(NVMainRequest* req);
    void enqueueShift(NVMainRequest* req);
    void enqueueImplicitPrecharge(NVMainRequest* req);
    void closeRow(NVMainRequest* req);
    bool handleCachedRequest(NVMainRequest* req);

    friend class RequestFinder;
    friend class RefreshHandler;
};

}; // namespace NVM

#endif

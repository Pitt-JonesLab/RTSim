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
 *
 * Racetrack/Domain wall memory support added by Asif Ali Khan in January 2019
 * Email: asif_ali.khan@tu-dresden.de
 *
 *******************************************************************************/

#include "MemControl/RTM/RTM.h"

#include "include/NVMainRequest.h"
#include "Logging/Logging.h"
#include "src/EventQueue.h"

#ifndef TRACE
#ifdef GEM5
#include "base/statistics.hh"
#include "base/types.hh"
#include "sim/core.hh"
#include "sim/stat_control.hh"
#include "SimInterface/Gem5Interface/Gem5Interface.h"

#endif
#endif
#include <assert.h>
#include <iostream>
#include <set>

using namespace NVM;
using namespace NVM::Logging;

RTM::RTM() {
    queueSize = 32;
    starvationThreshold = 4;
    averageLatency = 0.0f;
    averageQueueLatency = 0.0f;
    averageTotalLatency = 0.0f;
    measuredLatencies = 0;
    measuredQueueLatencies = 0;
    measuredTotalLatencies = 0;
    mem_reads = 0;
    mem_writes = 0;
    rb_hits = 0;
    rb_miss = 0;
    write_pauses = 0;
    starvation_precharges = 0;
    psInterval = 0;

    InitQueues(1);
    memQueue = &(transactionQueues[0]);
}

void RTM::SetConfig(Config* conf, bool createChildren) {
    if (conf->KeyExists("StarvationThreshold")) {
        starvationThreshold =
            static_cast<unsigned int>(conf->GetValue("StarvationThreshold"));
    }

    if (conf->KeyExists("QueueSize")) {
        queueSize = static_cast<unsigned int>(conf->GetValue("QueueSize"));
    }

    MemoryController::SetConfig(conf, createChildren);

    SetDebugName("RTM", conf);
}

void RTM::RegisterStats() {
    AddStat(mem_reads);
    AddStat(mem_writes);
    AddStat(rb_hits);
    AddStat(rb_miss);
    AddStat(starvation_precharges);
    AddStat(averageLatency);
    AddStat(averageQueueLatency);
    AddStat(averageTotalLatency);
    AddStat(measuredLatencies);
    AddStat(measuredQueueLatencies);
    AddStat(measuredTotalLatencies);
    AddStat(write_pauses);

    MemoryController::RegisterStats();
}

bool RTM::IsIssuable(NVMainRequest*, FailReason*) {
    return memQueue->size() < queueSize;
}

bool RTM::IssueCommand(NVMainRequest* req) {
    Logging::log() << LogLevel::DEBUG << "RTM - Received request at cycle "
                   << GetEventQueue()->GetCurrentCycle() << '\n';
    if (!IsIssuable(req)) {
        throw std::runtime_error("RTM is not issuable!");
        return false;
    }

    req->arrivalCycle = GetEventQueue()->GetCurrentCycle();

    Enqueue(0, req);

    if (req->type == READ) mem_reads++;
    else if (req->type == WRITE) mem_writes++;

    return true;
}

double updateAverage(double avg, double newVal, size_t totalMeasures) {
    return (avg * totalMeasures + newVal) / (totalMeasures + 1);
}

ncycle_t issueLatency(NVMainRequest* req) {
    return req->completionCycle - req->issueCycle;
}

ncycle_t queueLatency(NVMainRequest* req) {
    return req->issueCycle - req->arrivalCycle;
}

ncycle_t totalLatency(NVMainRequest* req) {
    return req->completionCycle - req->arrivalCycle;
}

bool RTM::RequestComplete(NVMainRequest* request) {
    if (request->type == WRITE || request->type == WRITE_PRECHARGE) {
        if (request->flags & NVMainRequest::FLAG_CANCELLED ||
            request->flags & NVMainRequest::FLAG_PAUSED) {
            Prequeue(0, request);

            return true;
        }
    }

    if (request->type == READ || request->type == READ_PRECHARGE ||
        request->type == WRITE || request->type == WRITE_PRECHARGE) {
        request->status = MEM_REQUEST_COMPLETE;
        request->completionCycle = GetEventQueue()->GetCurrentCycle();

        averageLatency = updateAverage(averageLatency, issueLatency(request),
                                       measuredLatencies);
        measuredLatencies += 1;

        averageQueueLatency = updateAverage(
            averageQueueLatency, queueLatency(request), measuredQueueLatencies);
        measuredQueueLatencies += 1;

        averageTotalLatency = updateAverage(
            averageTotalLatency, totalLatency(request), measuredTotalLatencies);
        measuredTotalLatencies += 1;
    }

    return MemoryController::RequestComplete(request);
}

void RTM::Cycle(ncycle_t steps) {
    NVMainRequest* nextRequest = nullptr;

    if (reqFinder.FindStarvedRequest(*memQueue, &nextRequest)) {
        rb_miss++;
        starvation_precharges++;
    } else if (reqFinder.FindRTMRowBufferHit(*memQueue, &nextRequest)) {
        rb_hits++;
    } else if (reqFinder.FindCachedAddress(*memQueue, &nextRequest)) {
    } else if (reqFinder.FindWriteStalledRead(*memQueue, &nextRequest)) {
        write_pauses++;
    } else if (reqFinder.FindOldestReadyRequest(*memQueue, &nextRequest)) {
        rb_miss++;
    } else if (reqFinder.FindClosedBankRequest(*memQueue, &nextRequest)) {
        rb_miss++;
    }

    if (nextRequest != NULL) {
        IssueMemoryCommands(nextRequest);
    }

    CycleCommandQueues();

    MemoryController::Cycle(steps);
}

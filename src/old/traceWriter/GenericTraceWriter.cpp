/*******************************************************************************
 * Copyright (c) 2012-2014, The Microsystems Design Labratory (MDL)
 * Department of Computer Science and Engineering, The Pennsylvania State
 *University All rights reserved.
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
 *******************************************************************************/

#include "src/old/traceWriter/GenericTraceWriter.h"

#include <assert.h>
#include <vector>


using namespace NVM;

GenericTraceWriter::GenericTraceWriter() :
    echo_on(false),
    perChannel(false),
    perRank(false) {}

GenericTraceWriter::~GenericTraceWriter() {}

void GenericTraceWriter::Init(Config* /*conf*/) {}

void GenericTraceWriter::SetEcho(bool echo) { echo_on = echo; }

bool GenericTraceWriter::GetEcho() { return echo_on; }

void GenericTraceWriter::SetPerChannelTraces(bool perChannel) {
    this->perChannel = perChannel;
}

void GenericTraceWriter::SetPerRankTraces(bool perRank) {
    this->perRank = perRank;
}

bool GenericTraceWriter::GetPerChannelTraces() { return this->perChannel; }

bool GenericTraceWriter::GetPerRankTraces() { return this->perRank; }

int GenericTraceWriter::SetNextNAccesses(
    unsigned int N, std::vector<TraceLine*>* nextAccesses) {
    int numWritten = 0;

    for (unsigned int lineIdx = 0; lineIdx < N; lineIdx++) {
        if (SetNextAccess(nextAccesses->at(lineIdx))) numWritten++;
    }

    return numWritten;
}

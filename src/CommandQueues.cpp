#include "src/CommandQueues.h"

#include "src/MemoryController.h"

#include <algorithm>
#include <cassert>
#include <limits>
#include <numeric>

using namespace NVM;

QueueHeadIterator& QueueHeadIterator::operator++() {
    if (end) return *this;

    current = (current + 1) % numQueues;
    if (current == startPos) {
        end = true;
        return *this;
    }

    while ((*queues)[current].empty()) {
        current = (current + 1) % numQueues;
        if (current == startPos) {
            end = true;
            return *this;
        }
    }
    return *this;
}

NVMainRequest* QueueHeadIterator::operator*() { return (*queues)[current][0]; }

bool QueueHeadIterator::operator==(QueueHeadIterator rhs) const {
    return queues == rhs.queues && current == rhs.current &&
           numQueues == rhs.numQueues && end == rhs.end &&
           startPos == rhs.startPos;
}

bool QueueHeadIterator::operator!=(QueueHeadIterator rhs) const {
    return !(*this == rhs);
}

ptrdiff_t QueueHeadIterator::operator-(QueueHeadIterator rhs) const {
    auto copy = *this;
    ptrdiff_t diff = 0;
    while (copy != rhs) {
        ++copy;
        diff++;
    }
    return diff;
}

CommandQueues::CommandQueues() {}

bool wasIssued(NVMainRequest* req) {
    return (req->flags & NVMainRequest::FLAG_ISSUED);
}

void CommandQueues::setNumQueues(size_t num) { numQueues = num; }

bool CommandQueues::effectivelyEmpty(size_t i) {
    bool effectivelyEmpty =
        (queues[i].size() == 1) && (wasIssued(queues[i].at(0)));

    return (queues[i].empty() || effectivelyEmpty);
}

bool CommandQueues::effectivelyEmpty() {
    std::vector<size_t> indices(queues.size());
    std::iota(indices.begin(), indices.end(), 0);
    return std::all_of(indices.begin(), indices.end(),
                       [&](auto i) { return effectivelyEmpty(i); });
}

bool CommandQueues::effectivelyEmpty(NVMAddress addr) {
    return effectivelyEmpty(getQueueIndex(addr));
}

void CommandQueues::setConfig(Config* config) {
    params.SetParams(config);
    if (config->KeyExists("MATHeight")) {
        numSubArrays = params.ROWS / params.MATHeight;
    } else {
        numSubArrays = 1;
    }

    queueModel = PerBankQueues;
    numQueues = params.RANKS * params.BANKS;
    if (config->KeyExists("QueueModel")) {
        if (config->GetString("QueueModel") == "PerRank") {
            queueModel = PerRankQueues;
            numQueues = params.RANKS;
        } else if (config->GetString("QueueModel") == "PerBank") {
            queueModel = PerBankQueues;
            numQueues = params.RANKS * params.BANKS;
        } else if (config->GetString("QueueModel") == "PerSubArray") {
            queueModel = PerSubArrayQueues;
            numQueues = params.RANKS * params.BANKS * numSubArrays;
        }
    }

    queues = std::vector<std::deque<NVMainRequest*>>(numQueues);
}

ncounter_t CommandQueues::getQueueIndex(NVMAddress addr) {
    ncounter_t queueId = std::numeric_limits<ncounter_t>::max();

    if (queueModel == PerRankQueues) {
        queueId = addr.GetRank();
    } else if (queueModel == PerBankQueues) {
        if (params.ScheduleScheme == 1) {
            queueId = (addr.GetBank() * params.RANKS + addr.GetRank());
        } else if (params.ScheduleScheme == 2) {
            queueId = (addr.GetRank() * params.BANKS + addr.GetBank());
        }
    } else if (queueModel == PerSubArrayQueues) {
        // TODO: ScheduleSchemes? There are 6 possible orderings now.
        queueId = (addr.GetRank() * params.BANKS * numSubArrays) +
                  (addr.GetBank() * numSubArrays) + addr.GetSubArray();
    }

    assert(queueId < numQueues);

    return queueId;
}

void CommandQueues::removeIssued() {
    for (ncycle_t queueId = 0; queueId < numQueues; queueId++) {
        queues[queueId].erase(std::remove_if(queues[queueId].begin(),
                                             queues[queueId].end(), wasIssued),
                              queues[queueId].end());
    }
}

void CommandQueues::enqueue(NVMainRequest* req) {
    auto i = getQueueIndex(req->address);
    queues[i].push_back(req);
}

bool CommandQueues::isEmpty(NVMAddress addr) {
    return queues[getQueueIndex(addr)].empty();
}

NVMainRequest* CommandQueues::peek(NVMAddress addr) {
    if (isEmpty(addr)) return nullptr;
    return queues[getQueueIndex(addr)][0];
}

NVMainRequest* CommandQueues::pop(NVMAddress addr) {
    auto front = peek(addr);
    if (front) queues[getQueueIndex(addr)].pop_front();
    return front;
}

void CommandQueues::checkForDeadlock(ncycle_t currentCycle) {
    for (int i = 0; i < numQueues; i++) {
        if (queues[i].empty()) continue;
        auto* queueHead = queues[i].at(0);

        if ((currentCycle - queueHead->issueCycle) > params.DeadlockTimer) {
            ncounter_t row, col, bank, rank, channel, subarray;
            queueHead->address.GetTranslatedAddress(&row, &col, &bank, &rank,
                                                    &channel, &subarray);
            std::cout << "NVMain Warning: Operation could not be sent to "
                         "memory after a very long time: "
                      << std::endl;
            std::cout << "         Address: 0x" << std::hex
                      << queueHead->address.GetPhysicalAddress() << std::dec
                      << " @ Bank " << bank << ", Rank " << rank << ", Channel "
                      << channel << " Subarray " << subarray << " Row " << row
                      << " Column " << col
                      << ". Queued time: " << queueHead->arrivalCycle
                      << ". Issue time: " << queueHead->issueCycle
                      << ". Current time: " << currentCycle
                      << ". Type: " << queueHead->type << std::endl;

            throw std::runtime_error("Deadlock Error");
        }
    }
}

size_t CommandQueues::size() { return queues.size(); }

size_t CommandQueues::size(NVMAddress addr) {
    return queues[getQueueIndex(addr)].size();
}

QueueHeadIterator CommandQueues::begin() { return begin(0); }

QueueHeadIterator CommandQueues::end() { return end(0); }

QueueHeadIterator CommandQueues::begin(size_t start) {
    size_t firstNonEmpty = start;
    while (queues[firstNonEmpty].empty()) {
        firstNonEmpty = (firstNonEmpty + 1) % numQueues;
        if (firstNonEmpty == start) return end(start);
    }

    return {&queues, firstNonEmpty, start, numQueues, false};
}

QueueHeadIterator CommandQueues::end(size_t start) {
    return {&queues, start, start, numQueues, true};
}
#pragma once

#include "include/NVMAddress.h"
#include "include/NVMainRequest.h"
#include "src/Config.h"
#include "src/Params.h"

#include <cstddef>
#include <iterator>

namespace NVM {

enum QueueModel { PerRankQueues, PerBankQueues, PerSubArrayQueues };

struct QueueHeadIterator {
    std::vector<std::deque<NVMainRequest*>>* queues;
    size_t current, startPos, numQueues;
    bool end;

    QueueHeadIterator& operator++();
    NVMainRequest* operator*();
    bool operator==(QueueHeadIterator rhs) const;
    bool operator!=(QueueHeadIterator rhs) const;
    ptrdiff_t operator-(QueueHeadIterator rhs) const;

    using difference_type = ptrdiff_t;
    using value_type = NVMainRequest*;
    using pointer = NVMainRequest**;
    using reference = NVMainRequest*&;
    using iterator_category = std::input_iterator_tag;
};

class CommandQueues {
    public:
    CommandQueues();

    void setConfig(Config* p);

    void setNumQueues(size_t numQueues);

    bool effectivelyEmpty(size_t i);
    bool effectivelyEmpty();
    bool effectivelyEmpty(NVMAddress addr);
    void removeIssued();

    void enqueue(NVMainRequest* req);

    bool isEmpty(NVMAddress addr);
    size_t size();
    size_t size(NVMAddress addr);

    NVMainRequest* peek(NVMAddress addr);
    NVMainRequest* pop(NVMAddress addr);

    void checkForDeadlock(ncycle_t currentCycle);

    ncounter_t getQueueIndex(NVMAddress addr);

    QueueHeadIterator begin();
    QueueHeadIterator begin(size_t start);
    QueueHeadIterator end();
    QueueHeadIterator end(size_t end);

    private:
    size_t numSubArrays;

    size_t numQueues;

    std::vector<std::deque<NVMainRequest*>> queues;

    Params params;
    QueueModel queueModel;
};

} // namespace NVM
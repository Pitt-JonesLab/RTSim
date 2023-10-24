#include "Memory/ChainedCommand.h"

using namespace NVM::Memory;

ChainedCommand::ChainedCommand(std::vector<std::function<Command*()>> s) : 
steps(s), parent(nullptr), child(nullptr), done(false) {
    getNextStep();
}

void ChainedCommand::setParent(Command* p) { parent = p; }

void ChainedCommand::notify() {
    child = nullptr;
    if (!steps.empty()) {
        getNextStep();
    } else {
        if (parent) parent->notify();
        done = true;
    }
}

bool ChainedCommand::isDone() const { return done; }

bool ChainedCommand::isBusy() const { return child; }

void ChainedCommand::getNextStep() { 
    Command* nextStep = steps[0]();
    if (nextStep) {
        steps.erase(steps.begin());
        child = nextStep;
        child->setParent(this);
    }
}

void ChainedCommand::cycle(unsigned int) {
    if (!isBusy() && !isDone()) getNextStep();
}
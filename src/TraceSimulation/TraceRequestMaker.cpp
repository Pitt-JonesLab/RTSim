#include "src/TraceSimulation/TraceRequestMaker.h"

#include "include/NVMainRequest.h"

using namespace NVM::Simulation;
using namespace NVM;

TraceRequestMaker::TraceRequestMaker(std::unique_ptr<TraceReader> reader) :
    reader(std::move(reader)) {}

NVM::NVMainRequest* TraceRequestMaker::getRequest() {
    convertNextTraceLine();
    if (nextReqs.empty()) return nullptr;
    auto req = nextReqs.front();
    nextReqs.pop();
    return req;
}

void TraceRequestMaker::convertNextTraceLine() {
    auto traceLine = reader->getLine();
    switch (traceLine.op1) {
        case Opcode1::READ:
            convertReadLine(traceLine);
            break;
        case Opcode1::WRITE:
            convertWriteLine(traceLine);
            break;
        case Opcode1::PIM:
            convertPIMLine(traceLine);
            break;
        case Opcode1::SHIFT:
            convertShiftLine(traceLine);
            break;
        case Opcode1::TRANSVERSE_WRITE:
            convertTWriteLine(traceLine);
            break;
        default:
            return;
    }
}

OpType opcodeToOptype(Opcode1 op1) {
    switch (op1) {
        case Opcode1::READ:
            return READ;
        case Opcode1::WRITE:
            return WRITE;
        case Opcode1::PIM:
            return PIM_OP;
        case Opcode1::SHIFT:
            return SHIFT;
        case Opcode1::TRANSVERSE_WRITE:
            return WRITE;
    }
}

NVMainRequest* makeRequest(TraceLine line) {
    NVMainRequest* req = new NVMainRequest();

    // TODO: TraceLine needs address
    req->type = opcodeToOptype(line.op1);
    req->bulkCmd = CMD_NOP;
    req->threadId = line.threadId;
    return req;
}

void TraceRequestMaker::convertReadLine(TraceLine line) {
    nextReqs.push(makeRequest(line));
}

void TraceRequestMaker::convertWriteLine(TraceLine line) {
    nextReqs.push(makeRequest(line));
}

void TraceRequestMaker::convertPIMLine(TraceLine line) {
    nextReqs.push(makeRequest(line));
}

void TraceRequestMaker::convertShiftLine(TraceLine line) {
    nextReqs.push(makeRequest(line));
}

void TraceRequestMaker::convertTWriteLine(TraceLine line) {
    nextReqs.push(makeRequest(line));
}
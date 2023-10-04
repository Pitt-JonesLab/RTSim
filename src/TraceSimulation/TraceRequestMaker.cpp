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

    NVMAddress address;
    address.SetPhysicalAddress(line.address1);
    req->address = address;

    NVMDataBlock data;
    data.SetSize(64);
    if (!line.data.empty()) {
        for (int i = 0; i < 64; i++) {
            data.SetByte(i, line.data[0].bytes[i]);
        }
    }
    req->data = data;

    NVMDataBlock oldData;
    oldData.SetSize(64);
    for (int i = 0; i < 64; i++) oldData.SetByte(i, 0);
    req->oldData = oldData;

    req->type = opcodeToOptype(line.op1);
    req->bulkCmd = CMD_NOP;
    req->threadId = line.threadId;
    req->arrivalCycle = line.cycle;
    req->status = MEM_REQUEST_INCOMPLETE;
    return req;
}

void TraceRequestMaker::convertReadLine(TraceLine line) {
    nextReqs.push(makeRequest(line));
}

void TraceRequestMaker::convertWriteLine(TraceLine line) {
    nextReqs.push(makeRequest(line));
}

void TraceRequestMaker::convertPIMLine(TraceLine line) {
    // nextReqs.push(makeRequest(line));
}

void TraceRequestMaker::convertShiftLine(TraceLine line) {
    // nextReqs.push(makeRequest(line));
}

void TraceRequestMaker::convertTWriteLine(TraceLine line) {
    // nextReqs.push(makeRequest(line));
}
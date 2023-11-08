#include "Simulation/TraceReader.h"

#include "NVMDataBlock.h"
#include "Simulation/ReadCommand.h"
#include "Simulation/RowCloneCommand.h"
#include "Simulation/TransverseReadCommand.h"
#include "Simulation/TransverseWriteCommand.h"
#include "Simulation/WriteCommand.h"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sstream>

using namespace NVM;
using namespace NVM::Simulation;

TraceReader::TraceReader(std::istream& trace) : trace(trace) {}

std::string getNextToken(std::istringstream& inStream) {
    std::string token;
    while (getline(inStream, token, ' ')) {
        if (token != "") return token;
    }
    return "";
}

unsigned int readCycle(std::istringstream& inStream) {
    return stoi(getNextToken(inStream));
}

Opcode1 readOperation(std::istringstream& inStream) {
    auto opString = getNextToken(inStream);
    if (opString == "R") return Opcode1::READ;
    if (opString == "W") return Opcode1::WRITE;
    if (opString == "P") return Opcode1::PIM;
    if (opString == "S") return Opcode1::SHIFT;
    if (opString == "T") return Opcode1::TRANSVERSE_WRITE;
    throw std::invalid_argument("Unknown operation in trace file!");
}

uint64_t readAddress(std::istringstream& inStream) {
    std::stringstream addressStream;
    addressStream << std::hex << getNextToken(inStream);
    uint64_t address;
    addressStream >> address;
    return address;
}

// TODO get this function under test
NVMDataBlock readData(std::istringstream& inStream) {
    auto field = getNextToken(inStream);

    if (field.length() != 128)
        throw std::invalid_argument("Invalid data block!");

    NVMDataBlock data;
    data.SetSize(64);

    uint32_t* rawData = reinterpret_cast<uint32_t*>(data.rawData);
    memset(rawData, 0, 64);

    for (int byte = 0; byte < 16; byte++) {
        std::stringstream fmat;

        fmat << std::hex << field.substr(byte * 8, 8);
        fmat >> rawData[byte];
        rawData[byte] = htonl(rawData[byte]);
    }
    return data;
}

std::unique_ptr<TraceCommand> TraceReader::getNext() {
    if (!trace.good()) return nullptr;
    if (trace.eof()) {
        std::cout << "TraceReader: Reached EOF!" << std::endl;
        return nullptr;
    }

    std::string fullLine;
    getline(trace, fullLine);

    std::istringstream lineStream(fullLine);

    unsigned int cycle;
    Opcode1 operation;
    uint64_t address;
    NVMDataBlock dataBlock;
    unsigned int threadId;

    try {
        cycle = readCycle(lineStream);
        operation = readOperation(lineStream);
        address = readAddress(lineStream);
        dataBlock = readData(lineStream);
        threadId = readCycle(lineStream);
    } catch (...) {
        std::cout << "Invalid trace format!\n";
        return nullptr;
    }

    DataBlock data;
    for (int i = 0; i < 64; i++) data.bytes[i] = dataBlock.GetByte(i);

    if (operation == Opcode1::TRANSVERSE_WRITE) {
        return std::unique_ptr<TraceCommand>(new TransverseWriteCommand());
    }
    if (operation == Opcode1::PIM) {
        getNextToken(lineStream);
        auto pimOp = getNextToken(lineStream);
        if (pimOp == "RC") {
            return std::unique_ptr<TraceCommand>(new RowCloneCommand());
        }
        return std::unique_ptr<TraceCommand>(new TransverseReadCommand());
    }

    switch (operation) {
        case Opcode1::READ:
            return std::unique_ptr<TraceCommand>(
                new ReadCommand(cycle, address, data, threadId));
        case Opcode1::WRITE:
            return std::unique_ptr<TraceCommand>(
                new WriteCommand(cycle, address, data, threadId));
        default:
            throw std::runtime_error("Unknown trace operation!");
    }
}
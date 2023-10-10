#include "Simulation/FileTraceReader.h"

#include "NVMDataBlock.h"
#include "Simulation/ReadCommand.h"
#include "Simulation/TraceCommand.h"
#include "Simulation/WriteCommand.h"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sstream>

using namespace NVM;
using namespace NVM::Simulation;

int readTraceVersion(std::ifstream& file) {
    std::string line;
    int currentPos = file.tellg();
    getline(file, line);

    if (line.substr(0, 4) == "NVMV") {
        std::string versionString = line.substr(4, std::string::npos);
        return atoi(versionString.c_str());
    }

    // file.seekg(currentPos, std::ios_base::beg);
    return 0;
}

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
    // if (opString == "P") return ROW_CLONE;
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

FileTraceReader::FileTraceReader(std::string traceFilename) :
    traceFile(traceFilename),
    traceVersion(-1) {}

std::unique_ptr<TraceCommand> FileTraceReader::getNext() {
    if (!traceFile.good()) return nullptr;
    if (traceVersion == -1) traceVersion = readTraceVersion(traceFile);
    std::string fullLine;
    getline(traceFile, fullLine);
    if (traceFile.eof()) {
        std::cout << "NVMainTraceReader: Reached EOF!" << std::endl;
        return nullptr;
    }

    std::istringstream lineStream(fullLine);

    unsigned int cycle = readCycle(lineStream);
    Opcode1 operation = readOperation(lineStream);
    uint64_t address = readAddress(lineStream);
    NVMDataBlock dataBlock = readData(lineStream);
    unsigned int threadId = readCycle(lineStream);

    DataBlock data;
    for (int i = 0; i < 64; i++) data.bytes[i] = dataBlock.GetByte(i);

    switch (operation) {
        case Opcode1::READ:
            return std::unique_ptr<TraceCommand>(
                new ReadCommand(cycle, address, data, threadId));
        case Opcode1::WRITE:
            return std::unique_ptr<TraceCommand>(
                new WriteCommand(cycle, address, data, threadId));
    }
}
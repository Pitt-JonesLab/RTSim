#include "src/TraceSimulation/FileTraceReader.h"

#include "include/NVMDataBlock.h"

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

TraceLine getNextLine(std::ifstream& trace) {
    std::string fullLine;
    getline(trace, fullLine);
    if (trace.eof()) {
        std::cout << "NVMainTraceReader: Reached EOF!" << std::endl;
        return {};
    }

    std::istringstream lineStream(fullLine);

    unsigned int cycle = readCycle(lineStream);
    Opcode1 operation = readOperation(lineStream);
    uint64_t address = readAddress(lineStream);
    NVMDataBlock dataBlock = readData(lineStream);
    unsigned int threadId = readCycle(lineStream);

    TraceLine::DataBlock data;
    for (int i = 0; i < 64; i++) data.bytes[i] = dataBlock.GetByte(i);
    return {cycle, operation, Opcode2::NONE, address, 0, {data}, threadId};
}

FileTraceReader::FileTraceReader(std::string traceFilename) :
    traceFile(traceFilename),
    traceVersion(-1) {}

TraceLine FileTraceReader::getLine() {
    if (!traceFile.good()) return {};
    if (traceVersion == -1) traceVersion = readTraceVersion(traceFile);
    return getNextLine(traceFile);
}
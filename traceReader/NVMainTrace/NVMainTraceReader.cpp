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

#include "traceReader/NVMainTrace/NVMainTraceReader.h"

#include <arpa/inet.h>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <ios>
#include <sstream>

using namespace NVM;

NVMainTraceReader::NVMainTraceReader() { traceVersion = -1; }

void NVMainTraceReader::SetTraceFile(std::string file) {
    traceFilename = file;
    trace.close();
    traceVersion = -1;
}

std::string NVMainTraceReader::GetTraceFile() { return traceFilename; }

void openTraceFile(std::ifstream& file, std::string fileName) {
    file.open(fileName);
    if (!file.is_open())
        throw std::runtime_error("Could not open trace file " + fileName);
}

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

OpType readOperation(std::istringstream& inStream) {
    auto opString = getNextToken(inStream);
    if (opString == "R") return READ;
    if (opString == "W") return WRITE;
    if (opString == "P") return ROW_CLONE;
    throw std::invalid_argument("Unknown operation in trace file!");
    return OpType(0);
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

bool NVMainTraceReader::GetNextAccess(TraceLine* nextAccess) {
    if (!trace.is_open()) openTraceFile(trace, traceFilename);
    if (traceVersion == -1) traceVersion = readTraceVersion(trace);

    std::string fullLine;
    getline(trace, fullLine);
    if (trace.eof()) {
        std::cout << "NVMainTraceReader: Reached EOF!" << std::endl;
        return false;
    }

    std::istringstream lineStream(fullLine);

    uint64_t address2;
    NVMDataBlock dataBlock;
    unsigned int cycle = readCycle(lineStream);
    OpType operation = readOperation(lineStream);
    uint64_t address = readAddress(lineStream);
    if (operation == ROW_CLONE) address2 = readAddress(lineStream);
    else dataBlock = readData(lineStream);

    unsigned int threadId = 0;
    NVMDataBlock oldDataBlock;
    if (traceVersion == 0) {
        threadId = readCycle(lineStream);
        oldDataBlock.SetSize(64);
        memset(oldDataBlock.rawData, 0, 64);
    } else {
        oldDataBlock = readData(lineStream);
        threadId = readCycle(lineStream);
    }

    NVMAddress nAddress;
    nAddress.SetPhysicalAddress(address);
    nextAccess->SetLine(nAddress, operation, cycle, dataBlock, oldDataBlock,
                        threadId);
    if (operation == ROW_CLONE) {
        NVMAddress destAddress;
        destAddress.SetPhysicalAddress(address2);
        nextAccess->setAddress2(destAddress);
    }

    return true;
}

int NVMainTraceReader::GetNextNAccesses(unsigned int N,
                                        std::vector<TraceLine*>* nextAccesses) {
    int successes = 0;

    for (unsigned int i = 0; i < N; i++) {
        TraceLine* nextLine = new TraceLine();
        if (GetNextAccess(nextLine)) {
            nextAccesses->push_back(nextLine);
            successes++;
        }
    }

    return successes;
}

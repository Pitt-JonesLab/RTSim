#include "Simulation/TraceReader/TraceReader.h"

#include "Logging/Logging.h"
#include "Simulation/Command/ECCCommand.h"
#include "Simulation/Command/ReadCommand.h"
#include "Simulation/Command/RowCloneCommand.h"
#include "Simulation/Command/TransverseReadCommand.h"
#include "Simulation/Command/TransverseWriteCommand.h"
#include "Simulation/Command/WriteCommand.h"
#include "src/old/NVMDataBlock.h"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sstream>

using namespace NVM;
using namespace NVM::Simulation;

enum class Opcode1 { READ, WRITE, PIM, TRANSVERSE_WRITE, SHIFT, NONE };

enum class Opcode2 { BITWISE, ROWCLONE, NONE };

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

Opcode2 readOp2(std::istringstream& inStream) {
    auto opString = getNextToken(inStream);
    if (opString == "RC") return Opcode2::ROWCLONE;
    if (opString == "AND") return Opcode2::BITWISE;
    if (opString == "OR") return Opcode2::BITWISE;
    if (opString == "XOR") return Opcode2::BITWISE;
    if (opString == "NAND") return Opcode2::BITWISE;
    if (opString == "NOR") return Opcode2::BITWISE;
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

    if (field.find_first_of("0x") == 0)
        field = field.substr(2, field.size() - 1);

    if (field.size() % 8 != 0) {
        auto padding = 8 - (field.size() % 8);
        for (int i = 0; i < padding; i++) field = '0' + field;
    }

    NVMDataBlock data;
    data.SetSize(64);

    uint32_t* rawData = reinterpret_cast<uint32_t*>(data.rawData);
    memset(rawData, 0, 64);

    for (int byte = 0; byte < field.size() / 8; byte++) {
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

    if (fullLine == "ECC_FAIL")
        return std::unique_ptr<TraceCommand>(new ECCCommand());

    std::istringstream lineStream(fullLine);

    unsigned int cycle;
    Opcode1 operation;
    Opcode2 op2;
    uint64_t address, address2;
    NVMDataBlock dataBlock;
    unsigned int threadId;

    try {
        cycle = readCycle(lineStream);
        Logging::log() << Logging::LogLevel::DEBUG << std::dec << "Read cycle "
                       << cycle << '\n';
        operation = readOperation(lineStream);
        Logging::log() << Logging::LogLevel::DEBUG << "Read opcode "
                       << (int) operation << '\n';
        address = readAddress(lineStream);
        Logging::log() << Logging::LogLevel::DEBUG << "Read address "
                       << std::hex << address << '\n';
        if (operation == Opcode1::PIM) {
            op2 = readOp2(lineStream);
            Logging::log() << Logging::LogLevel::DEBUG << "Read op2 "
                           << (int) op2 << '\n';
            address2 = readAddress(lineStream);
            Logging::log() << Logging::LogLevel::DEBUG << "Read address2 "
                           << std::hex << address2 << '\n';
        }
        dataBlock = readData(lineStream);
        threadId = readCycle(lineStream);
    } catch (...) {
        std::cout << "Invalid trace format!\n";
        return nullptr;
    }

    DataBlock data;
    for (int i = 0; i < 64; i++) data.bytes[i] = dataBlock.GetByte(i);

    switch (operation) {
        case Opcode1::READ:
            return std::unique_ptr<TraceCommand>(
                new ReadCommand(cycle, address, data, threadId));
        case Opcode1::WRITE:
            return std::unique_ptr<TraceCommand>(
                new WriteCommand(cycle, address, data, threadId));
        case Opcode1::PIM: {
            if (op2 == Opcode2::ROWCLONE) {
                return std::unique_ptr<TraceCommand>(
                    new RowCloneCommand(address, address2, data));
            }
            return std::unique_ptr<TraceCommand>(
                new TransverseReadCommand(address, address2));
        }
        default:
            throw std::runtime_error("Unknown trace operation!");
    }
}

Command TraceReader::getNextCommand() {
    if (!trace.good()) Command();
    if (trace.eof()) {
        std::cout << "TraceReader: Reached EOF!" << std::endl;
        return Command();
    }

    std::string fullLine;
    getline(trace, fullLine);

    // if (fullLine == "ECC_FAIL")
    //     return std::unique_ptr<TraceCommand>(new ECCCommand());

    std::istringstream lineStream(fullLine);

    unsigned int cycle;
    Opcode1 operation;
    Opcode2 op2;
    Address address, address2;
    NVMDataBlock dataBlock;
    unsigned int threadId;

    try {
        cycle = readCycle(lineStream);
        Logging::log() << Logging::LogLevel::DEBUG << std::dec << "Read cycle "
                       << cycle << '\n';
        operation = readOperation(lineStream);
        Logging::log() << Logging::LogLevel::DEBUG << "Read opcode "
                       << (int) operation << '\n';
        address = Address(readAddress(lineStream));
        // Logging::log() << Logging::LogLevel::DEBUG << "Read address "
        //                << std::hex << address << '\n';
        if (operation == Opcode1::PIM) {
            op2 = readOp2(lineStream);
            Logging::log() << Logging::LogLevel::DEBUG << "Read op2 "
                           << (int) op2 << '\n';
            address2 = Address(readAddress(lineStream));
            // Logging::log() << Logging::LogLevel::DEBUG << "Read address2 "
            //                << std::hex << address2 << '\n';
        }
        dataBlock = readData(lineStream);
        threadId = readCycle(lineStream);
    } catch (...) {
        std::cout << "Invalid trace format!\n";
        return nullptr;
    }

    RowData data;
    for (int i = 0; i < 64; i++) data.setByte(i, dataBlock.GetByte(i));

    switch (operation) {
        case Opcode1::READ:
            return [address, data](Commandable& receiver) -> bool {
                return receiver.read(address, data);
            };
        // case Opcode1::WRITE:
        //     return std::unique_ptr<TraceCommand>(
        //         new WriteCommand(cycle, address, data, threadId));
        // case Opcode1::PIM: {
        //     if (op2 == Opcode2::ROWCLONE) {
        //         return std::unique_ptr<TraceCommand>(
        //             new RowCloneCommand(address, address2, data));
        //     }
        //     return std::unique_ptr<TraceCommand>(
        //         new TransverseReadCommand(address, address2));
        // }
        default:
            throw std::runtime_error("Unknown trace operation!");
    }
}
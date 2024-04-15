#include "Simulation/TraceReader/TraceReader.h"

#include "Logging/Logging.h"
#include "src/old/NVMDataBlock.h"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sstream>

using namespace NVM;
using namespace NVM::Simulation;

using NVM::Memory::MemorySystem;

enum class Opcode1 { READ, WRITE, PIM, TRANSVERSE_WRITE, SHIFT, NONE };

std::ostream& operator<<(std::ostream& out, Opcode1 op1) {
    switch (op1) {
        case Opcode1::READ:
            out << "READ";
            break;
        case Opcode1::WRITE:
            out << "WRITE";
            break;
        case Opcode1::PIM:
            out << "PIM";
            break;
        case Opcode1::TRANSVERSE_WRITE:
            out << "TRANS_WRITE";
            break;
        case Opcode1::SHIFT:
            out << "SHIFT";
            break;
        case Opcode1::NONE:
            out << "???";
            break;
    }
    return out;
}

enum class Opcode2 { BITWISE, ROWCLONE, NONE };

std::ostream& operator<<(std::ostream& out, Opcode2 op2) {
    switch (op2) {
        case Opcode2::BITWISE:
            out << "TRANS_READ";
            break;
        case Opcode2::ROWCLONE:
            out << "ROWCLONE";
            break;
        case Opcode2::NONE:
            out << "???";
            break;
    }
    return out;
}

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

    if (field.find("0x") == 0) field = field.substr(2, field.size() - 1);

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

TraceReader::Command TraceReader::getNextCommand() {
    if (!trace.good()) Command();
    if (trace.eof()) {
        std::cout << "TraceReader: Reached EOF!" << std::endl;
        return Command();
    }

    std::string fullLine;
    getline(trace, fullLine);

    // End on empty line
    if (fullLine.size() == 0) return nullptr;

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
                       << operation << '\n';

        address = Address(readAddress(lineStream));
        Logging::log() << Logging::LogLevel::DEBUG << "Read dest address 0x"
                       << std::hex << address.getData() << std::dec << '\n';

        if (operation == Opcode1::PIM) {
            op2 = readOp2(lineStream);
            Logging::log() << Logging::LogLevel::DEBUG << "Read op2 " << op2
                           << '\n';

            address2 = Address(readAddress(lineStream));
            Logging::log() << Logging::LogLevel::DEBUG << "Read src address 0x"
                           << std::hex << address2.getData() << std::dec
                           << '\n';
        } else if (operation == Opcode1::SHIFT) {
            auto op2 = getNextToken(lineStream);
            Logging::log() << Logging::LogLevel::DEBUG << "Read shift opcode "
                           << op2 << '\n';
        } else if (operation == Opcode1::TRANSVERSE_WRITE) {
            auto op2 = getNextToken(lineStream);
            Logging::log() << Logging::LogLevel::DEBUG
                           << "Read transverse write opcode " << op2 << '\n';
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
            return [address, data](MemorySystem& receiver) -> bool {
                return receiver.read(address, data);
            };
        case Opcode1::WRITE:
            return [address, data](MemorySystem& receiver) -> bool {
                return receiver.write(address, data);
            };
        case Opcode1::PIM:
            if (op2 == Opcode2::ROWCLONE) {
                return
                    [address, address2, data](MemorySystem& receiver) -> bool {
                        return receiver.rowClone(address, address2, data);
                    };
            }
            return [address, address2, data](MemorySystem& receiver) -> bool {
                return receiver.pim({address}, address2, {data});
            };
        case Opcode1::SHIFT:
            return [](MemorySystem& receiver) -> bool { return true; };
        case Opcode1::TRANSVERSE_WRITE:
            return [address, data](MemorySystem& receiver) -> bool {
                return receiver.transverseWrite(address, data);
            };
        default:
            throw std::runtime_error("Unknown trace operation!");
    }
}
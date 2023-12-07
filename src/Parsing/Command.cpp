#include "Parsing/Command.h"

using namespace NVM::Parsing;

Command::Command(CommandType type, Address address) :
    type(type),
    address(address) {}

CommandType Command::getType() const { return type; }

Address Command::getAddress() const { return address; }
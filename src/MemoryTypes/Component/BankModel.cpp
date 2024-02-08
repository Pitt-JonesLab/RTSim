#include "MemoryTypes/Component/BankModel.h"

NVM::ComponentType::BankModel::BankModel() : openRow(-1) {}

int NVM::ComponentType::BankModel::getOpenRow() const { return openRow; }

void NVM::ComponentType::BankModel::setOpenRow(int row) { openRow = row; }

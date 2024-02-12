#pragma once

namespace NVM::ComponentType {

class BankModel {
    public:
    BankModel();

    int getOpenRow() const;

    void setOpenRow(int row);

    private:
    int openRow;
};

} // namespace NVM::ComponentType
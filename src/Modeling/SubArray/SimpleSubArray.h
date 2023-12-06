#include "Modeling/SubArray.h"

namespace NVM::Modeling {

class SimpleSubArray : public SubArray {
    public:
    SimpleSubArray();

    RowNumber getOpenRow() const;
    bool isOpen() const;
    void closeRow();
    void openRow(RowNumber row);

    private:
    bool open;
    RowNumber row;
};

} // namespace NVM::Modeling
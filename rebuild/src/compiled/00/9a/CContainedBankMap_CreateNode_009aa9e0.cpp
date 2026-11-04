// Retail node allocation calls malloc (BFEA0E), paired with native erase/free.
#include "fable_retail_banks.h"
#include <new>

NBankFileManager::CContainedBankMap::CNode*
NBankFileManager::CContainedBankMap::CreateNode(const value_type& value)
{
    CNode* node = static_cast<CNode*>(malloc(sizeof(CNode)));
    void* valueAddress = &node->value_;
    if (valueAddress != 0)
        new (valueAddress) value_type(value);
    return node;
}

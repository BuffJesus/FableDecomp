#include "fable_retail_banks.h"

// NBankFileManager::CContainedBankMap::operator[] @ 0x009AC530.
NBankFileManager::CContainedBank&
NBankFileManager::CContainedBankMap::operator[](const CCharString& key)
{
    iterator where = lower_bound(key);
    if (where == end() || key < where->first)
        where = insert(where, value_type(key, CContainedBank()));
    return where->second;
}

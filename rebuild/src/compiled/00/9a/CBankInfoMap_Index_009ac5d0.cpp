#include "fable_retail_banks.h"

CBankFileManager::CBankInfo&
CBankFileManager::CBankInfoMap::operator[](const CCharString& key)
{
    iterator where = lower_bound(key);
    if (where == end() || key < where->first)
        where = insert(where, value_type(key, CBankInfo()));
    return where->second;
}

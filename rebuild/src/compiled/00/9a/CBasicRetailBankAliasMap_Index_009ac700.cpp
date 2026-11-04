#include "fable_retail_banks.h"

CCharString& CBasicRetailBankAliasMap::operator[](const CCharString& key)
{
    iterator where = lower_bound(key);
    if (where == end() || key < where->first)
        where = insert(where, value_type(key, CCharString()));
    return where->second;
}

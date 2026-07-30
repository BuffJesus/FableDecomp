#include <stdio.h>
#include <string.h>

#include "fable_retail_banks.h"

namespace
{
    CCharString g_slot;
    const CCharString* g_key = 0;
    const CCharString* g_value = 0;
}

CCharString& CBasicRetailBankAliasMap::operator[](
    const CCharString& key)
{
    g_key = &key;
    return g_slot;
}

CCharString& CCharString::operator=(const CCharString& other)
{
    g_value = &other;
    return *this;
}

CCharString::CCharString()
{
}

CCharString::~CCharString()
{
}

fable_i32 g_CCharStringInstanceCount_013BD800 = 0;

int main()
{
    CBasicRetailBankManager manager;
    CCharString genericName;
    CCharString platformName;

    g_key = 0;
    g_value = 0;
    manager.AddBankAlias(genericName, platformName);

    if (g_key != &platformName || g_value != &genericName)
    {
        printf("FABLETLC_RETAIL_BANK_ALIAS_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    printf("FABLETLC_RETAIL_BANK_ALIAS_BEHAVIOR PASS\n");
    return 0;
}

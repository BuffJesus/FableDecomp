#include <stdio.h>
#include "fable_retail_banks.h"

extern CBankFileManager g_FableBankFileManager_013CA79C;
extern CBankFileManager g_013ca79c;

static int constructed;
static int destroyed;
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBankFileManager::CBankFileManager() : CBaseClass()
{
    ++constructed;
    retailBankVersion_ = 100;
}
CBankFileManager::~CBankFileManager() { ++destroyed; }

int main()
{
    if (sizeof(CBankFileManager) != 0x38 || constructed != 1 || destroyed != 0)
        return 1;
    if (&g_FableBankFileManager_013CA79C != &g_013ca79c ||
        g_013ca79c.retailBankVersion_ != 100)
        return 2;
    puts("BANK_MANAGER_GLOBAL_PASS");
    return 0;
}

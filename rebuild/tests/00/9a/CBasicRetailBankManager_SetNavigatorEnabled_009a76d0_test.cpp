#include <stdio.h>
#include <string.h>

#include "fable_retail_banks.h"

int main()
{
    CBasicRetailBankManager manager;
    memset(&manager, 0xA5, sizeof(manager));

    manager.SetNavigatorEnabled(true);
    if (!manager.navigatorEnabled_)
    {
        printf("FABLETLC_RETAIL_BANK_NAVIGATOR_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    manager.SetNavigatorEnabled(false);
    if (manager.navigatorEnabled_)
    {
        printf("FABLETLC_RETAIL_BANK_NAVIGATOR_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_RETAIL_BANK_NAVIGATOR_BEHAVIOR PASS\n");
    return 0;
}

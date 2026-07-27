#include <stdio.h>

#include "rebuild_abi.h"

struct CTCInventoryBaseQuickAssignView
{
    fable_u8 unknown0000[0x3A54];
    bool canItemBeQuickAssigned3A54;
};

bool FABLE_FASTCALL
FableGetCanItemBeQuickAssigned_00A0AA60(
    const CTCInventoryBaseQuickAssignView* inventory);

int main()
{
    CTCInventoryBaseQuickAssignView inventory = {};

    inventory.canItemBeQuickAssigned3A54 = true;
    if (!FableGetCanItemBeQuickAssigned_00A0AA60(&inventory))
    {
        printf("FABLETLC_INVENTORY_QUICK_ASSIGN_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    inventory.canItemBeQuickAssigned3A54 = false;
    if (FableGetCanItemBeQuickAssigned_00A0AA60(&inventory))
    {
        printf("FABLETLC_INVENTORY_QUICK_ASSIGN_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_INVENTORY_QUICK_ASSIGN_BEHAVIOR PASS\n");
    return 0;
}

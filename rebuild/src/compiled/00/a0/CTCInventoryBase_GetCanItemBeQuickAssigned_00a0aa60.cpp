#include "rebuild_abi.h"

struct CTCInventoryBaseQuickAssignView
{
    fable_u8 unknown0000[0x3A54];
    bool canItemBeQuickAssigned3A54;
};

bool FABLE_FASTCALL
FableGetCanItemBeQuickAssigned_00A0AA60(
    const CTCInventoryBaseQuickAssignView* inventory)
{
    return inventory->canItemBeQuickAssigned3A54;
}

#include <stdio.h>

struct COwner
{
    void Release(int slot);
};

struct CTCInventoryItem
{
    int    field0;
    int    slot;
    int    field8;
    int    fieldC;
    COwner* owner;
    void RemoveFromInventory();
};

static int g_released_slot = -999;
static int g_release_calls = 0;

void COwner::Release(int slot)
{
    g_released_slot = slot;
    g_release_calls++;
}

void CTCInventoryItem::RemoveFromInventory()
{
    if (this->owner != 0)
    {
        this->owner->Release(this->slot);
        this->owner = 0;
    }
}

int main()
{
    COwner owner;
    CTCInventoryItem item;
    item.field0 = 0; item.slot = 77; item.field8 = 0; item.fieldC = 0;
    item.owner = &owner;

    // case 1: owner present -> Release(slot) called, owner nulled
    item.RemoveFromInventory();
    bool ok1 = (g_release_calls == 1) && (g_released_slot == 77) && (item.owner == 0);

    // case 2: owner null -> no call, stays null
    g_release_calls = 0;
    item.RemoveFromInventory();
    bool ok2 = (g_release_calls == 0) && (item.owner == 0);

    if (ok1 && ok2)
        printf("REMOVE_INV_OK\n");
    else
        printf("REMOVE_INV_FAIL c=%d s=%d o=%p\n", g_release_calls, g_released_slot, (void*)item.owner);
    return (ok1 && ok2) ? 0 : 1;
}
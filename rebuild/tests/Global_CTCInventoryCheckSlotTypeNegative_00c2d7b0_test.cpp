// Standalone behaviour test for CTCInventory_CheckSlotTypeNegative.
// Verifies the switch logic and the +0x0E field layout against a controlled
// fixture. Prints the PassPattern on success.
#include <cstdio>
#include <cstddef>

struct CTCInventory
{
    unsigned char  pad00[0x0E];
    unsigned short slotType;
};

int __fastcall CTCInventory_CheckSlotTypeNegative(CTCInventory* self)
{
    switch (self->slotType)
    {
    case 0:  return 0;
    case 1:  return -1;
    case 2:  return -1;
    default: return 0;
    }
}

int main()
{
    CTCInventory inv;
    for (int i = 0; i < 0x0E; ++i) inv.pad00[i] = 0xCC; // poison pad to catch misreads

    const unsigned short in[]  = { 0, 1, 2, 3, 7, 65535 };
    const int            exp[] = { 0, -1, -1, 0, 0, 0 };

    bool ok = true;
    for (int i = 0; i < 6; ++i)
    {
        inv.slotType = in[i];
        int r = CTCInventory_CheckSlotTypeNegative(&inv);
        if (r != exp[i])
        {
            ok = false;
            std::printf("MISMATCH in=%u got=%d exp=%d\n", in[i], r, exp[i]);
        }
    }

    if (ok)
        std::printf("CTCINVENTORY_CHECKSLOTTYPENEGATIVE_TEST PASS\n");
    return ok ? 0 : 1;
}
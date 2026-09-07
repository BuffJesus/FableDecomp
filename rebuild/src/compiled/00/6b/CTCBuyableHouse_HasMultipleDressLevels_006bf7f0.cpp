// CTCBuyableHouse::HasMultipleDressLevels  @ 0x006bf7f0

#include "engine/CTCBuyableHouse.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDefPointeeBase {
    char pad0[0x28];
    int* first;   // +0x28
    int* last;    // +0x2c
};


bool __fastcall HasMultipleDressLevels(CTCBuyableHouse* self)
{
    CDefPointeeBase* d = self->PDef_Object;
    if (d != 0)
    {
        unsigned int count = (unsigned int)(d->last - d->first);
        if (count > 1)
            return true;
    }
    return false;
}
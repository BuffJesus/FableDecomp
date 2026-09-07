#pragma optimize("s",on)

#include "engine/CInventoryItemDef.h"  // retyped onto the PDB layout; byte parity re-verified
unsigned long __fastcall CSpecialAbilitiesDrainLifeDef_GetSizeofClass(const CInventoryItemDef* self)
{
    (void)self;
    return 0x70;
}
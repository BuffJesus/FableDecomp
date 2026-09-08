#pragma optimize("s",on)
#include "engine/CEnvironmentThemeDaySetDef.h"  // retyped onto the PDB layout; byte parity re-verified

// Retail returns 0x6c: the retail class ends at EditorColour (+0x6c..+0x70) and does NOT
// carry the Ego_r donor's trailing FishWeightMult (+0x70), so sizeof(header) == 0x74 would
// change the immediate. Keep the retail constant, as the other landed GetSizeofClass do.
unsigned long __fastcall CEnvironmentThemeDaySetDef_GetSizeofClass(const CEnvironmentThemeDaySetDef* self)
{
    (void)self;
    return 0x6c;
}

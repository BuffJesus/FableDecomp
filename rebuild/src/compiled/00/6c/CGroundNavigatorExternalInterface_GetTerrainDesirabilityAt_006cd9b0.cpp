// CGroundNavigatorExternalInterface_Methods::GetTerrainDesirabilityAt @ 0x004df3cb
// Retail:
//   fld dword ptr ds:[0x122ded8]   ; return global float
//   ret 4                          ; __fastcall: this in ecx, one 4-byte stack arg
//
// The function ignores 'this' and its single argument and just returns a
// global float value. Model it as a __fastcall member with one 4-byte
// parameter so the compiler emits 'ret 4'.

#include "engine/CGroundNavigatorExternalInterface.h"  // retyped onto the PDB layout; byte parity re-verified
extern float g_terrainDesirability; // ds:0x122ded8 (reloc-masked)

struct CGroundNavigatorExternalInterface_Methods : CGroundNavigatorExternalInterface {
    float GetTerrainDesirabilityAt(void* pos);
};

float CGroundNavigatorExternalInterface_Methods::GetTerrainDesirabilityAt(void* pos)
{
    (void)pos;
    return g_terrainDesirability;
}
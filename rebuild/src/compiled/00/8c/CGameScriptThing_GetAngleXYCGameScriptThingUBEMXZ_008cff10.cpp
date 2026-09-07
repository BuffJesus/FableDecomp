// ?GetAngleXY@CGameScriptThing@@UBEMXZ @ 0x008cff10
// Retail: d9 41 40 c3  =>  fld DWORD PTR [ecx+0x40] ; ret

#include "engine/CGameScriptThing.h"  // retyped onto the PDB layout; byte parity re-verified
float __fastcall CGameScriptThing__GetAngleXY(const CGameScriptThing* this_ptr)
{
    return this_ptr->AngleXY;
}
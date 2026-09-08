#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

void __fastcall CGameScriptInterface_SetAllowScreenFadingIfAlreadyFaded(
    CGameScriptInterface* self, int /*edx*/, char value)
{
    // Retail stores the incoming byte verbatim; assigning through bool would
    // make VC7.1 normalize it to 0/1 and change the instruction sequence.
    *(char*)&self->AllowScreenFadingIfAlreadyFaded = value;
}

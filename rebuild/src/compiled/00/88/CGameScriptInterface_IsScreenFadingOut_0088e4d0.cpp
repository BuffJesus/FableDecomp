#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDisplayEngine { char pad0[0xbc]; unsigned char b_bc; char pad1[0xc]; unsigned char b_c9; };

int __fastcall CGameScriptInterface_IsScreenFadingOut(const CGameScriptInterface* self)
{
    const CDisplayEngine* s = self->DisplayEngine;
    if (s->b_bc) {
        if (s->b_c9) {
            return 1;
        }
    }
    return 0;
}
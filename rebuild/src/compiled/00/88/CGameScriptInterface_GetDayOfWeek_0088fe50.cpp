#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Inner2 { char _p[0x14]; long f14; };
struct CWorld { char _p[0x1c]; Inner2* p1c; };

long __fastcall CGameScriptInterface_GetDayOfWeek(const CGameScriptInterface* self)
{
    CWorld* m = self->World;
    Inner2* x = m->p1c;
    return x->f14 % 7;
}
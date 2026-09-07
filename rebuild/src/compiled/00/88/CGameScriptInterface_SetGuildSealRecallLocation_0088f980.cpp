#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Vec3 { int x; int y; int z; };
struct CWorld { char pad[0x154]; Vec3 loc; int extra; };

void __fastcall CGameScriptInterface_SetGuildSealRecallLocation(
    CGameScriptInterface* self, int edx_dummy, const Vec3* loc, int val)
{
    (void)edx_dummy;
    Vec3* d = &self->World->loc;
    d->x = loc->x;
    d->y = loc->y;
    d->z = loc->z;
    self->World->extra = val;
}
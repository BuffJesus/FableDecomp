struct Vec3 { int x; int y; int z; };
struct CMid { char pad[0x154]; Vec3 loc; int extra; };
struct CGameScriptInterface { void* vt; CMid* mid; };

void __fastcall CGameScriptInterface_SetGuildSealRecallLocation(
    CGameScriptInterface* self, int edx_dummy, const Vec3* loc, int val)
{
    (void)edx_dummy;
    Vec3* d = &self->mid->loc;
    d->x = loc->x;
    d->y = loc->y;
    d->z = loc->z;
    self->mid->extra = val;
}
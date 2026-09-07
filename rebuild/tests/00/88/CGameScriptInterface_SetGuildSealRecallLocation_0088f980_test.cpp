#include "engine/CGameScriptInterface.h"
#include <cstdio>

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

int main() {
    CWorld midObj;
    CGameScriptInterface obj;
    obj.World = &midObj;
    Vec3 loc; loc.x = 11; loc.y = 22; loc.z = 33;
    CGameScriptInterface_SetGuildSealRecallLocation(&obj, 0, &loc, 7);
    if (midObj.loc.x == 11 && midObj.loc.y == 22 && midObj.loc.z == 33 && midObj.extra == 7) {
        std::printf("CGameScriptInterface_0088f980_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}
#include "engine/CGameScriptInterface.h"
#include <cstdio>

struct Inner2 { char _p[0x14]; long f14; };
struct CWorld { char _p[0x1c]; Inner2* p1c; };

long __fastcall CGameScriptInterface_GetDayOfWeek(const CGameScriptInterface* self)
{
    CWorld* m = self->World;
    Inner2* x = m->p1c;
    return x->f14 % 7;
}

int main()
{
    Inner2 x; x.f14 = 100;
    CWorld m; m.p1c = &x;
    CGameScriptInterface o; o.World = &m;
    long r = CGameScriptInterface_GetDayOfWeek(&o);
    if (r != 100 % 7) { std::printf("FAIL got %ld\n", r); return 1; }
    x.f14 = -8;
    r = CGameScriptInterface_GetDayOfWeek(&o);
    if (r != -8 % 7) { std::printf("FAIL neg got %ld\n", r); return 1; }
    std::printf("CGameScriptInterface_0088fe50_TEST PASS\n");
    return 0;
}
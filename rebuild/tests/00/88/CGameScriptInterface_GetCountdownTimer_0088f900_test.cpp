#include <cstdio>

struct CountdownSub { char pad[0xa0]; int frames; };
struct CGameScriptInterface { void* pad0; CountdownSub* sub; };

float __fastcall CGameScriptInterface_GetCountdownTimer(const CGameScriptInterface* self)
{
    return (float)self->sub->frames;
}

int main()
{
    CountdownSub s;
    s.frames = 4200;
    CGameScriptInterface gsi;
    gsi.pad0 = 0;
    gsi.sub = &s;
    float r = CGameScriptInterface_GetCountdownTimer(&gsi);
    if (r != 4200.0f) { std::printf("FAIL: got %f\n", r); return 1; }
    s.frames = -7;
    r = CGameScriptInterface_GetCountdownTimer(&gsi);
    if (r != -7.0f) { std::printf("FAIL: got %f\n", r); return 1; }
    std::printf("CGameScriptInterface_0088f900_TEST PASS\n");
    return 0;
}
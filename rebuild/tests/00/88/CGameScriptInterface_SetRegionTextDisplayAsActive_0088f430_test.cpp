#include <cstdio>
struct CMid { unsigned char _pad[0xdf]; unsigned char regionTextActive; };
struct CGameScriptInterface { void* vt; CMid* mid; };

void __fastcall CGameScriptInterface_SetRegionTextDisplayAsActive(CGameScriptInterface* self, void* edx, char active)
{
    self->mid->regionTextActive = (unsigned char)active;
}

int main()
{
    CMid theMid;
    theMid.regionTextActive = 0;
    CGameScriptInterface gsi;
    gsi.vt = 0;
    gsi.mid = &theMid;
    CGameScriptInterface_SetRegionTextDisplayAsActive(&gsi, 0, 1);
    if (theMid.regionTextActive != 1) { std::printf("FAIL expected 1 got %d\n", (int)theMid.regionTextActive); return 1; }
    CGameScriptInterface_SetRegionTextDisplayAsActive(&gsi, 0, 0);
    if (theMid.regionTextActive != 0) { std::printf("FAIL expected 0 got %d\n", (int)theMid.regionTextActive); return 1; }
    std::printf("CGameScriptInterface_0088f430_TEST PASS\n");
    return 0;
}
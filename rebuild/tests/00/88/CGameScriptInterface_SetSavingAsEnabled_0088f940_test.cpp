#include <cstdio>

struct CMid { unsigned char pad[0xd7]; unsigned char flagD7; };
struct CGameScriptInterface { void* vt; CMid* mid; };

void __fastcall CGameScriptInterface_SetSavingAsEnabled(CGameScriptInterface* self, int edx_dummy, char enabled)
{
    self->mid->flagD7 = (unsigned char)enabled;
}

int main()
{
    CMid midObj;
    midObj.flagD7 = 0;
    CGameScriptInterface obj;
    obj.vt = 0;
    obj.mid = &midObj;
    CGameScriptInterface_SetSavingAsEnabled(&obj, 0, 1);
    if (midObj.flagD7 != 1) { std::printf("FAIL set 1\n"); return 1; }
    CGameScriptInterface_SetSavingAsEnabled(&obj, 0, 0);
    if (midObj.flagD7 != 0) { std::printf("FAIL set 0\n"); return 1; }
    std::printf("CGameScriptInterface_0088f940_TEST PASS\n");
    return 0;
}
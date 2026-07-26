#include <cstdio>

int g_someInt = 100;

struct CGameScriptInterface {
    char pad[0x48];
    int field48;
};

void __fastcall CGameScriptInterface_CameraResetToViewBehindHero(CGameScriptInterface* self, void* edx, float scale)
{
    long v = (long)((double)g_someInt * scale);
    self->field48 = v;
    if (v < 0) self->field48 = 0;
}

int main()
{
    CGameScriptInterface obj;
    obj.field48 = -999;

    CGameScriptInterface_CameraResetToViewBehindHero(&obj, 0, 2.5f);
    if (obj.field48 != 250) { std::printf("FAIL pos got %d\n", obj.field48); return 1; }

    CGameScriptInterface_CameraResetToViewBehindHero(&obj, 0, -3.0f);
    if (obj.field48 != 0) { std::printf("FAIL neg got %d\n", obj.field48); return 1; }

    CGameScriptInterface_CameraResetToViewBehindHero(&obj, 0, 0.0f);
    if (obj.field48 != 0) { std::printf("FAIL zero got %d\n", obj.field48); return 1; }

    std::printf("CGameScriptInterface_00890fc0_TEST PASS\n");
    return 0;
}
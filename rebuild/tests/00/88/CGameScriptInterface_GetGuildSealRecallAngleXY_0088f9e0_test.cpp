#include <cstdio>

struct Inner { char pad[0x160]; float angle; };
struct CGameScriptInterface { void* vtbl; Inner* inner; };

float __fastcall CGameScriptInterface_GetGuildSealRecallAngleXY(const CGameScriptInterface* self)
{
    return self->inner->angle;
}

int main()
{
    Inner box;
    box.angle = 42.5f;
    CGameScriptInterface obj;
    obj.vtbl = 0;
    obj.inner = &box;
    float r = CGameScriptInterface_GetGuildSealRecallAngleXY(&obj);
    if (r == 42.5f) {
        std::printf("CGameScriptInterface_0088f9e0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %f\n", r);
    return 1;
}
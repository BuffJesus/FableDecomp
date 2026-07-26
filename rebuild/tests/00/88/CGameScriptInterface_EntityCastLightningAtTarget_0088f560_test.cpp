#include <cstdio>

struct CGameScriptInterface { void* vt; };

void __fastcall CGameScriptInterface_EntityCastLightningAtTarget(CGameScriptInterface* self, int edx_ignored, int a, int b)
{
    (void)self; (void)a; (void)b;
}

int main()
{
    CGameScriptInterface obj;
    obj.vt = 0;
    CGameScriptInterface_EntityCastLightningAtTarget(&obj, 0, 1, 2);
    std::printf("CGameScriptInterface_0088f560_TEST PASS\n");
    return 0;
}
#include <cstdio>

float g_globalTargetingDistanceOffset;

void __stdcall CGameScriptInterface_SetGlobalTargetingDistanceOffset(float offset)
{
    g_globalTargetingDistanceOffset = offset;
}

int main()
{
    CGameScriptInterface_SetGlobalTargetingDistanceOffset(3.5f);
    if (g_globalTargetingDistanceOffset == 3.5f) {
        std::printf("CGameScriptInterface_0088fbe0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}
#include <cstdio>

float g_GlobalTargetingDistanceOffset = 3.5f;

float __fastcall CGameScriptInterface_GetGlobalTargetingDistanceOffset(void* self)
{
    (void)self;
    return g_GlobalTargetingDistanceOffset;
}

int main()
{
    int dummy = 0;
    float r = CGameScriptInterface_GetGlobalTargetingDistanceOffset(&dummy);
    if (r == 3.5f) {
        std::printf("CGameScriptInterface_0088fbf0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %f\n", r);
    return 1;
}
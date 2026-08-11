#include <cstdio>

struct CGlobalState
{
    char pad[0x4694];
    int spotlightShadowMode;
};

CGlobalState* g_engineGlobal;

struct CEngine
{
    void SetSpotlightShadowMode(bool enable);
};

void CEngine::SetSpotlightShadowMode(bool enable)
{
    g_engineGlobal->spotlightShadowMode = (enable ? 1 : 0) + 1;
}

int main()
{
    static CGlobalState st;
    g_engineGlobal = &st;
    CEngine e;

    e.SetSpotlightShadowMode(false);
    if (st.spotlightShadowMode != 1) { printf("FAIL off=%d\n", st.spotlightShadowMode); return 1; }

    e.SetSpotlightShadowMode(true);
    if (st.spotlightShadowMode != 2) { printf("FAIL on=%d\n", st.spotlightShadowMode); return 1; }

    printf("SPOTLIGHT_SHADOW_OK\n");
    return 0;
}
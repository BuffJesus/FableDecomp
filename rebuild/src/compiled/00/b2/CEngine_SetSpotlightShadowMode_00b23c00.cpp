// CEngine::SetSpotlightShadowMode @ 0x00b23c00
// this (ecx) is ignored; bool arg on stack; writes 1+(bool!=0) to *(int*)(g+0x4694)

struct CGlobalState
{
    char pad[0x4694];
    int spotlightShadowMode;
};

extern CGlobalState* g_engineGlobal;
CGlobalState* g_engineGlobal;

struct CEngine
{
    void SetSpotlightShadowMode(bool enable);
};

void CEngine::SetSpotlightShadowMode(bool enable)
{
    g_engineGlobal->spotlightShadowMode = (enable ? 1 : 0) + 1;
}
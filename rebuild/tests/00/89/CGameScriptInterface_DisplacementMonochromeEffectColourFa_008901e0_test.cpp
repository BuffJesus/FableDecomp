#include <cstdio>

static void* g_seen_self;
static int g_a, g_b, g_c;

extern "C" void* __fastcall Effect_ColourFadeOut(void* self, void* edx, int a, int b, int c)
{
    g_seen_self = self; g_a = a; g_b = b; g_c = c;
    return self;
}

void* __fastcall CGameScriptInterface_DisplacementMonochromeEffectColourFadeOut(void* self, void* edx, int a, int b, int c)
{
    void* pWorld  = *(void**)((char*)self  + 0x4);
    void* pEffMgr = *(void**)((char*)pWorld + 0x18);
    void* pEffect = *(void**)((char*)pEffMgr + 0x1964);
    return Effect_ColourFadeOut(pEffect, edx, a, b, c);
}

int main()
{
    /* Build the pointer chain: self+4 -> world; world+0x18 -> mgr; mgr+0x1964 -> effect. */
    static char effectRegion[16];
    void* effect = (void*)&effectRegion[0];

    static char mgrRegion[0x1964 + 8];
    *(void**)(mgrRegion + 0x1964) = effect;
    void* mgr = (void*)&mgrRegion[0];

    static char worldRegion[0x18 + 8];
    *(void**)(worldRegion + 0x18) = mgr;
    void* world = (void*)&worldRegion[0];

    static char selfRegion[0x4 + 8];
    *(void**)(selfRegion + 0x4) = world;
    void* self = (void*)&selfRegion[0];

    void* ret = CGameScriptInterface_DisplacementMonochromeEffectColourFadeOut(self, (void*)0, 11, 22, 33);

    if (g_seen_self == effect && ret == effect && g_a == 11 && g_b == 22 && g_c == 33) {
        std::printf("CGameScriptInterface_008901e0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL self=%p a=%d b=%d c=%d\n", g_seen_self, g_a, g_b, g_c);
    return 1;
}
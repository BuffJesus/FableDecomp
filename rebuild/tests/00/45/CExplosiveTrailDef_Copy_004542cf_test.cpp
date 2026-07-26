#include <cstdio>

struct CExplosiveTrailDef { int tag; };

static int g_called = 0;
static const void* g_src = 0;
static CExplosiveTrailDef* g_self = 0;

void __fastcall CExplosiveTrailDef_CopyImpl(CExplosiveTrailDef* self, const void* src)
{
    g_called++;
    g_self = self;
    g_src = src;
}

void __fastcall CExplosiveTrailDef_Copy(CExplosiveTrailDef* self, const void* src)
{
    CExplosiveTrailDef_CopyImpl(self, src);
}

int main()
{
    CExplosiveTrailDef def; def.tag = 7;
    int marker = 0x1234;
    CExplosiveTrailDef_Copy(&def, &marker);
    if (g_called == 1 && g_self == &def && g_src == (const void*)&marker) {
        std::printf("CExplosiveTrailDef_004542cf_TEST PASS\n");
        return 0;
    }
    std::printf("CExplosiveTrailDef_004542cf_TEST FAIL\n");
    return 1;
}
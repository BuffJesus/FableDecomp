#include "engine/CFireballSpellLevelDef.h"
#include <cstdio>

struct CDefClassBase;
struct CFireballSpellLevelDef_Methods : CFireballSpellLevelDef {
    void InheritFrom(CDefClassBase const* src);
    void Copy(CDefClassBase const* src);
};

static void* g_self = 0;
static const void* g_src = 0;
void CFireballSpellLevelDef_Methods::InheritFrom(CDefClassBase const* src)
{
    g_self = this;
    g_src = src;
}

int main()
{
    CFireballSpellLevelDef_Methods dst; dst.SpellLevel = 0;
    CFireballSpellLevelDef_Methods srcObj; srcObj.SpellLevel = 0xDEADBEEF;
    dst.Copy((CDefClassBase const*)&srcObj);
    if (dst.SpellLevel == (long)0xDEADBEEF && g_self == &dst && g_src == (void*)&srcObj)
        printf("004e0f34_TEST PASS\n");
    else
        printf("FAIL %lx\n", dst.SpellLevel);
    return 0;
}

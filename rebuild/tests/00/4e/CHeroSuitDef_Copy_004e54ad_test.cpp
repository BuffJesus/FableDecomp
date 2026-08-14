#include <stdio.h>

struct SubDef { int v; };
struct CHeroSuitDef {
    char basepad[0x28];
    SubDef sub;
};

static int g_baseCalls = 0;
static int g_subCalls = 0;
static void* g_lastBaseSelf = 0;
static const void* g_lastBaseSrc = 0;
static void* g_lastSubSelf = 0;
static const void* g_lastSubSrc = 0;

static void BaseCopy(void* self, const void* src) {
    g_baseCalls++; g_lastBaseSelf = self; g_lastBaseSrc = src;
}
static void SubCopy(SubDef* self, const SubDef* src) {
    g_subCalls++; g_lastSubSelf = self; g_lastSubSrc = src;
    self->v = src->v;
}

static void HeroSuitCopy(CHeroSuitDef* self, const CHeroSuitDef* src) {
    BaseCopy(self, src);
    SubCopy((SubDef*)((char*)self + 0x28),
            (const SubDef*)((const char*)src + 0x28));
}

int main() {
    CHeroSuitDef dst; CHeroSuitDef src;
    src.sub.v = 12345;
    dst.sub.v = 0;
    HeroSuitCopy(&dst, &src);
    int ok = 1;
    if (g_baseCalls != 1) ok = 0;
    if (g_subCalls != 1) ok = 0;
    if (g_lastBaseSelf != (void*)&dst) ok = 0;
    if (g_lastBaseSrc != (const void*)&src) ok = 0;
    if (g_lastSubSelf != (void*)((char*)&dst + 0x28)) ok = 0;
    if (g_lastSubSrc != (const void*)((const char*)&src + 0x28)) ok = 0;
    if (dst.sub.v != 12345) ok = 0;
    if (ok) printf("BEHAVIOUR_OK\n");
    else printf("BEHAVIOUR_FAIL\n");
    return 0;
}
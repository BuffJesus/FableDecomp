#include <cstdio>

struct CThingMultiArrowShotDef { int v; };

static int g_calls = 0;
static CThingMultiArrowShotDef* g_self = 0;
static const CThingMultiArrowShotDef* g_other = 0;

void __fastcall CThingMultiArrowShotDef_CopyBase(CThingMultiArrowShotDef* self, void* edx, const CThingMultiArrowShotDef* other)
{
    (void)edx;
    g_calls++;
    g_self = self;
    g_other = other;
    self->v = other->v;
}

void __fastcall CThingMultiArrowShotDef_Copy(CThingMultiArrowShotDef* self, void* edx, const CThingMultiArrowShotDef* other)
{
    CThingMultiArrowShotDef_CopyBase(self, edx, other);
}

int main()
{
    CThingMultiArrowShotDef a; a.v = 0;
    CThingMultiArrowShotDef b; b.v = 42;
    CThingMultiArrowShotDef_Copy(&a, 0, &b);
    if (g_calls == 1 && g_self == &a && g_other == &b && a.v == 42) {
        std::printf("CThingMultiArrowShotDef_004536a9_TEST PASS\n");
        return 0;
    }
    std::printf("CThingMultiArrowShotDef_004536a9_TEST FAIL\n");
    return 1;
}
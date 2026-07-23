#include <cstdio>

struct CCoopSpiritDef { int v; };

static int g_called = 0;
static CCoopSpiritDef* g_self = 0;
static const CCoopSpiritDef* g_other = 0;

void __fastcall Base_Copy(CCoopSpiritDef* self, const CCoopSpiritDef* other)
{
    g_called = 1;
    g_self = self;
    g_other = other;
    self->v = other->v;
}

void __fastcall CCoopSpiritDef_Copy(CCoopSpiritDef* self, const CCoopSpiritDef* other)
{
    Base_Copy(self, other);
}

int main()
{
    CCoopSpiritDef a; a.v = 7;
    CCoopSpiritDef b; b.v = 99;
    CCoopSpiritDef_Copy(&b, &a);
    if (g_called == 1 && g_self == &b && g_other == &a && b.v == 7) {
        std::printf("CCoopSpiritDef_00453be3_TEST PASS\n");
        return 0;
    }
    std::printf("CCoopSpiritDef_00453be3_TEST FAIL\n");
    return 1;
}
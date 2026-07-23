#include <cstdio>

struct CBonusItemDef { int v; };

static int g_called = 0;
static CBonusItemDef* g_self = 0;
static const CBonusItemDef* g_src = 0;

void __fastcall CBonusItemDef_CopyImpl(CBonusItemDef* self, const CBonusItemDef* src)
{
    g_called = 1;
    g_self = self;
    g_src = src;
    self->v = src->v;
}

void __fastcall CBonusItemDef_Copy(CBonusItemDef* self, const CBonusItemDef* src)
{
    CBonusItemDef_CopyImpl(self, src);
}

int main()
{
    CBonusItemDef a; a.v = 0;
    CBonusItemDef b; b.v = 42;
    CBonusItemDef_Copy(&a, &b);
    if (g_called == 1 && g_self == &a && g_src == &b && a.v == 42) {
        std::printf("CBonusItemDef_00453c53_TEST PASS\n");
        return 0;
    }
    std::printf("CBonusItemDef_00453c53_TEST FAIL\n");
    return 1;
}
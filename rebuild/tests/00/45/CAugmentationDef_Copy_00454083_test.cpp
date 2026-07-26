#include <cstdio>

struct CAugmentationDef { int v; };

static int g_called = 0;
static CAugmentationDef* g_self = 0;
static const CAugmentationDef* g_src = 0;

void __fastcall AugmentationDef_CopyImpl(CAugmentationDef* self, const CAugmentationDef* src)
{
    g_called = 1;
    g_self = self;
    g_src = src;
    self->v = src->v;
}

void __fastcall CAugmentationDef_Copy(CAugmentationDef* self, const CAugmentationDef* src)
{
    AugmentationDef_CopyImpl(self, src);
}

int main()
{
    CAugmentationDef a; a.v = 0;
    CAugmentationDef b; b.v = 42;
    CAugmentationDef_Copy(&a, &b);
    if (g_called == 1 && g_self == &a && g_src == &b && a.v == 42) {
        std::printf("CAugmentationDef_00454083_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}
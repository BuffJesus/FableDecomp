#include <cstdio>
struct CJackDragonDef { int v; };

static int g_called = 0;
static CJackDragonDef* g_self = 0;
static const CJackDragonDef* g_src = 0;

void __fastcall CJackDragonDef_CopyImpl(CJackDragonDef* self, void* dummy, const CJackDragonDef* src)
{
    (void)dummy;
    g_called = 1;
    g_self = self;
    g_src = src;
    self->v = src->v;
}

void __fastcall CJackDragonDef_Copy(CJackDragonDef* self, void* dummy, const CJackDragonDef* src)
{
    CJackDragonDef_CopyImpl(self, dummy, src);
}

int main()
{
    CJackDragonDef a; a.v = 0;
    CJackDragonDef b; b.v = 77;
    CJackDragonDef_Copy(&a, 0, &b);
    if (g_called && g_self == &a && g_src == &b && a.v == 77) {
        std::printf("CJackDragonDef_004540ec_TEST PASS\n");
        return 0;
    }
    std::printf("CJackDragonDef_004540ec_TEST FAIL\n");
    return 1;
}
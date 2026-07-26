#include <cstdio>

struct CTattooDef { int v; };

static int g_called = 0;
static CTattooDef* g_self = 0;
static const CTattooDef* g_src = 0;

void __fastcall CTattooDef_CopyImpl(CTattooDef* self, void* /*edx*/, const CTattooDef* src)
{
    g_called++;
    g_self = self;
    g_src = src;
    self->v = src->v;
}

void __fastcall CTattooDef_Copy(CTattooDef* self, void* edx, const CTattooDef* src)
{
    CTattooDef_CopyImpl(self, edx, src);
}

int main()
{
    CTattooDef a; a.v = 0;
    CTattooDef b; b.v = 0x1234;
    CTattooDef_Copy(&a, 0, &b);
    if (g_called == 1 && g_self == &a && g_src == &b && a.v == 0x1234) {
        std::printf("CTattooDef_00453d41_TEST PASS\n");
        return 0;
    }
    std::printf("CTattooDef_00453d41_TEST FAIL\n");
    return 1;
}
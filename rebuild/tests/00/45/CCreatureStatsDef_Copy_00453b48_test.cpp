#include <cstdio>

struct CCreatureStatsDef { int v; };

static int g_called = 0;
static CCreatureStatsDef* g_self = 0;
static const CCreatureStatsDef* g_src = 0;

void __fastcall CCreatureStatsDef_CopyImpl(CCreatureStatsDef* self, void* dummy, const CCreatureStatsDef* src)
{
    (void)dummy;
    g_called = 1;
    g_self = self;
    g_src = src;
}

void __fastcall CCreatureStatsDef_Copy(CCreatureStatsDef* self, void* dummy, const CCreatureStatsDef* src)
{
    CCreatureStatsDef_CopyImpl(self, dummy, src);
}

int main()
{
    CCreatureStatsDef a; a.v = 1;
    CCreatureStatsDef b; b.v = 2;
    CCreatureStatsDef_Copy(&a, 0, &b);
    if (g_called == 1 && g_self == &a && g_src == &b) {
        std::printf("CCreatureStatsDef_00453b48_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}
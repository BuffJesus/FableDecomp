#include <cstdio>

struct CBriarRoseDef { int v; };

static CBriarRoseDef* g_last_self = 0;
static const CBriarRoseDef* g_last_src = 0;

void __fastcall CBriarRoseDef_CopyImpl(CBriarRoseDef* self, const CBriarRoseDef* src)
{
    g_last_self = self;
    g_last_src = src;
    self->v = src->v;
}

void __fastcall CBriarRoseDef_Copy(CBriarRoseDef* self, const CBriarRoseDef* src)
{
    CBriarRoseDef_CopyImpl(self, src);
}

int main()
{
    CBriarRoseDef a; a.v = 0;
    CBriarRoseDef b; b.v = 42;
    CBriarRoseDef_Copy(&a, &b);
    if (a.v == 42 && g_last_self == &a && g_last_src == &b) {
        std::printf("CBriarRoseDef_0045417e_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}
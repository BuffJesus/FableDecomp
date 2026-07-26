#include <cstdio>

struct CActionUseDef { int v; };

static int g_called = 0;
static CActionUseDef* g_self = 0;
static const CActionUseDef* g_other = 0;

void __fastcall CActionUseDef_Copy_impl(CActionUseDef* self, void* edx_unused, const CActionUseDef* other)
{
    (void)edx_unused;
    g_called = 1;
    g_self = self;
    g_other = other;
    self->v = other->v;
}

void __fastcall CActionUseDef_Copy(CActionUseDef* self, void* edx_unused, const CActionUseDef* other)
{
    CActionUseDef_Copy_impl(self, edx_unused, other);
}

int main()
{
    CActionUseDef dst; dst.v = 0;
    CActionUseDef src; src.v = 42;
    CActionUseDef_Copy(&dst, 0, &src);
    if (g_called == 1 && g_self == &dst && g_other == &src && dst.v == 42) {
        std::printf("CActionUseDef_00453899_TEST PASS\n");
        return 0;
    }
    std::printf("CActionUseDef_00453899_TEST FAIL\n");
    return 1;
}
#include <cstdio>
struct CSnowTrollDef { int v; };
static int g_called = 0;
void __fastcall CSnowTrollDef_CopyImpl(CSnowTrollDef* self, const CSnowTrollDef* other)
{
    g_called = 1;
    self->v = other->v;
}
void __fastcall CSnowTrollDef_Copy(CSnowTrollDef* self, const CSnowTrollDef* other)
{
    CSnowTrollDef_CopyImpl(self, other);
}
int main()
{
    CSnowTrollDef a; a.v = 0;
    CSnowTrollDef b; b.v = 42;
    CSnowTrollDef_Copy(&a, &b);
    if (g_called == 1 && a.v == 42) {
        std::printf("CSnowTrollDef_004541da_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}
#include <cstdio>

static int g_dtor = 0;
static int g_del = 0;
static void* g_last = 0;

struct CDecal { int dummy; };

static void Dtor(CDecal* self) { g_dtor++; }
static void OpDelete(CDecal* self) { g_del++; g_last = self; }

static void Delete(CDecal* self)
{
    if (self)
    {
        Dtor(self);
        OpDelete(self);
    }
}

int main()
{
    Delete(0);
    if (g_dtor != 0 || g_del != 0) { printf("FAIL null\n"); return 1; }

    CDecal d; d.dummy = 7;
    Delete(&d);
    if (g_dtor != 1 || g_del != 1 || g_last != &d) { printf("FAIL nonnull\n"); return 1; }

    printf("OK_00af7ca0\n");
    return 0;
}
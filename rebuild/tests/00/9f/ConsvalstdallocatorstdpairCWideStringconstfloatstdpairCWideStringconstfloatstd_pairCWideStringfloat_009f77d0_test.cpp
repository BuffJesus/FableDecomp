#include <cstdio>

struct Pair;
static int g_calls = 0;
static Pair* g_last = 0;

extern "C" void __cdecl destroy_helper(Pair* p) { g_calls++; g_last = p; }

struct AllocT { char dummy; };

void __fastcall cons_val(AllocT* self, int edxDummy, Pair* dest, Pair* val)
{
    (void)self; (void)edxDummy; (void)val;
    if (dest != 0)
    {
        destroy_helper(dest);
    }
}

int main()
{
    AllocT a;
    Pair* p = (Pair*)0x1234;
    // null dest -> no call
    cons_val(&a, 0, 0, 0);
    if (g_calls != 0) { printf("FAIL null\n"); return 1; }
    // non-null dest -> one call with dest
    cons_val(&a, 0, p, (Pair*)0x9999);
    if (g_calls != 1 || g_last != p) { printf("FAIL nonnull\n"); return 1; }
    printf("CONSVAL_OK\n");
    return 0;
}
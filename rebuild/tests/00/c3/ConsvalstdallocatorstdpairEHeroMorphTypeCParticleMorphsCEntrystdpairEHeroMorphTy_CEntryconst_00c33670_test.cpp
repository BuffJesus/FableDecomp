#include <cstdio>

struct Alloc {};
struct Pair {};

static Alloc* g_seenSelf;
static void*  g_seenFirst;
static void*  g_seenLast;

extern "C" void __fastcall RealConsVal(Alloc* self, void* first, void* last)
{
    g_seenSelf  = self;
    g_seenFirst = first;
    g_seenLast  = last;
}

Alloc g_allocInstance;

void __fastcall Cons_val_forwarder(Alloc* /*self*/, Pair* first, Pair* last)
{
    RealConsVal(&g_allocInstance, first, last);
}

int main()
{
    Pair a, b;
    Alloc dummy;
    Cons_val_forwarder(&dummy, &a, &b);

    bool ok = (g_seenSelf == &g_allocInstance)
           && (g_seenFirst == (void*)&a)
           && (g_seenLast  == (void*)&b);
    if (ok) { printf("CONS_VAL_FWD_OK\n"); return 0; }
    printf("CONS_VAL_FWD_FAIL\n"); return 1;
}
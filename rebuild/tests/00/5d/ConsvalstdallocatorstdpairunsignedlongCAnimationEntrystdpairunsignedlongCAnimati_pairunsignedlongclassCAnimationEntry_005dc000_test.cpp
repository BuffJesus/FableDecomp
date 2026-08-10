#include <cstdio>

struct Pair;
struct PairV;

// Capture what the forwarder passes to the callee.
static void*   g_self;
static PairV*  g_a3;
static Pair*   g_a1;
static PairV*  g_a2;
static int     g_called;

struct Pair  { int x; };
struct PairV { int y; };

extern "C" void __fastcall inner_cons(void* self, PairV* a3, Pair* a1, PairV* a2)
{
    g_self=self; g_a3=a3; g_a1=a1; g_a2=a2; g_called=1;
}

struct Cons { void* m0; };

void __fastcall _Cons_val(Cons* self, int, Pair* a1, PairV* a2, PairV* a3)
{
    inner_cons(self->m0, a3, a1, a2);
}

int main()
{
    int innerObj = 0x1234;
    Cons c; c.m0 = &innerObj;
    Pair  p1; PairV p2; PairV p3;
    _Cons_val(&c, 0, &p1, &p2, &p3);

    bool ok = g_called
        && g_self == &innerObj
        && g_a1 == &p1
        && g_a2 == &p2
        && g_a3 == &p3;
    if (ok) printf("005dc000_TEST PASS\n");
    else    printf("005dc000_TEST FAIL self=%p a1=%p a2=%p a3=%p\n", g_self,(void*)g_a1,(void*)g_a2,(void*)g_a3);
    return 0;
}
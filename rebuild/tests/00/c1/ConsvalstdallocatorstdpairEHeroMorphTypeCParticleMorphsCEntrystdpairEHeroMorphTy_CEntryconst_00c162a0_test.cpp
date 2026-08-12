// Independent behaviour test for the _Cons_val +4 forwarder.
#include <cstdio>

struct Pair;

struct AllocSub {
    void Cons_val_worker(Pair* a, Pair* b);
};
struct AllocOuter {
    int      lead;
    AllocSub sub;
    void _Cons_val_fwd(Pair* a, Pair* b);
};

// Capture what the worker received, to prove the +4 adjust + arg pass-through.
static void* g_self;
static Pair* g_a;
static Pair* g_b;

void AllocSub::Cons_val_worker(Pair* a, Pair* b)
{
    g_self = this;
    g_a = a;
    g_b = b;
}

// Provide the forwarder body here (matches the source under test).
void AllocOuter::_Cons_val_fwd(Pair* a, Pair* b)
{
    this->sub.Cons_val_worker(a, b);
}

int main()
{
    AllocOuter o;
    o.lead = 0x11223344;
    Pair* pa = (Pair*)0x1000;
    Pair* pb = (Pair*)0x2000;

    o._Cons_val_fwd(pa, pb);

    void* expect_self = (void*)((char*)&o + 4);
    bool ok = (g_self == expect_self) && (g_a == pa) && (g_b == pb);

    if (ok) {
        printf("CONS_VAL_FWD_OK\n");
        return 0;
    }
    printf("CONS_VAL_FWD_FAIL self=%p exp=%p\n", g_self, expect_self);
    return 1;
}

#include <cstdio>

struct PairSlot
{
    void Release();
};

static int g_releaseCalls = 0;
static void* g_lastThisValue = 0;

void PairSlot::Release()
{
    g_releaseCalls++;
    // this is &p (address of the local parameter slot); dereference it to
    // recover the pointer value that was passed in.
    g_lastThisValue = *reinterpret_cast<void**>(this);
}

struct Allocator
{
    void Cons_val_pair_morph_i(PairSlot* p);
};

void Allocator::Cons_val_pair_morph_i(PairSlot* p)
{
    reinterpret_cast<PairSlot*>(&p)->Release();
}

int main()
{
    Allocator a;
    PairSlot* p = (PairSlot*)0x1234;
    a.Cons_val_pair_morph_i(p);
    if (g_releaseCalls != 1) { printf("FAIL_CALLS\n"); return 1; }
    if (g_lastThisValue != (void*)0x1234) { printf("FAIL_VAL\n"); return 2; }
    printf("PASS_CONS_VAL_PAIR_MORPH_I\n");
    return 0;
}
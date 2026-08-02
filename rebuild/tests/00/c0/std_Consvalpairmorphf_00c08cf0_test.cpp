#include <cstdio>

struct PairMorph
{
    int a;
    int b;
    void Construct();
};

static int g_calls = 0;
static void* g_lastSelf = 0;

void PairMorph::Construct()
{
    g_calls++;
    g_lastSelf = this;
    a = 42;
}

extern "C" void __stdcall Cons_val_pair_morph_f(PairMorph* _Ptr)
{
    _Ptr->Construct();
}

int main()
{
    PairMorph m;
    m.a = 0;
    Cons_val_pair_morph_f(&m);
    void* mp = &m;
    if (g_calls == 1 && g_lastSelf == mp && m.a == 42) {
        std::printf("PAIRMORPH_CONS_OK\n");
        return 0;
    }
    std::printf("PAIRMORPH_CONS_FAIL calls=%d a=%d\n", g_calls, m.a);
    return 1;
}
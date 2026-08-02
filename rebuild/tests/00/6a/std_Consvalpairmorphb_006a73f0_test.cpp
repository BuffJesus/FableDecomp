#include <cstdio>

struct MorphPair;

static int g_called = 0;
static void* g_lastSelf = 0;

extern "C" void __fastcall MorphPair_CtorHelper(MorphPair* self)
{
    g_called = 1;
    g_lastSelf = (void*)self;
}

extern "C" void __stdcall Cons_val_pair_morph_b(MorphPair* _Ptr)
{
    MorphPair_CtorHelper(_Ptr);
}

int main()
{
    int dummy = 0;
    MorphPair* p = (MorphPair*)&dummy;

    Cons_val_pair_morph_b(p);

    if (g_called == 1 && g_lastSelf == (void*)p)
    {
        printf("CONS_VAL_PAIR_MORPH_B_OK\n");
    }
    else
    {
        printf("CONS_VAL_PAIR_MORPH_B_FAIL\n");
    }
    return 0;
}
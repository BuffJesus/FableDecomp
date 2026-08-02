
#include <cstdio>

static void* g_lastSelf = 0;

extern "C" void __fastcall Real_Cons_val_pair_morph_c(void* self)
{
    g_lastSelf = self;
}

extern "C" void __stdcall Cons_val_pair_morph_c(void* ptr)
{
    Real_Cons_val_pair_morph_c(ptr);
}

int main()
{
    int marker_val = 0x1234;
    void* p = (void*)&marker_val;

    Cons_val_pair_morph_c(p);

    if (g_lastSelf == p)
    {
        std::printf("THUNK_FORWARD_OK\n");
    }
    else
    {
        std::printf("THUNK_FORWARD_FAIL\n");
    }
    return 0;
}
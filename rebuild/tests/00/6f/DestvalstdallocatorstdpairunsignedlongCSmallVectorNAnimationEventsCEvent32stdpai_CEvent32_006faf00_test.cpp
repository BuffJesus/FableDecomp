#include <cstdio>

struct SmallVec {
    int   f0;
    void* f4;
    int   f8;
};

static void* g_freed = (void*)0;

extern "C" void __cdecl free_bfeb1c(void* p) { g_freed = p; }

void __fastcall _Dest_val_pair_ulong_CSmallVector(SmallVec* self);

int main()
{
    int dummy = 0;
    SmallVec v;
    v.f0 = 111;
    v.f4 = &dummy;   // pretend heap buffer
    v.f8 = 42;

    _Dest_val_pair_ulong_CSmallVector(&v);

    if (g_freed == &dummy && v.f4 == (void*)0 && v.f8 == 0 && v.f0 == 111)
        printf("006faf00_TEST PASS\n");
    else
        printf("006faf00_TEST FAIL freed=%p f4=%p f8=%d f0=%d\n", g_freed, v.f4, v.f8, v.f0);
    return 0;
}
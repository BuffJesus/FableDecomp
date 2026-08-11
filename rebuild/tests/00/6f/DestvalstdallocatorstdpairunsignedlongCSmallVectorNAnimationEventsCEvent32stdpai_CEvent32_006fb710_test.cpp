#include <cstdio>

struct DestVal {
    void* pad0;
    void* buf;
    void* end;
};

static void* g_freed = (void*)0xDEAD;

extern "C" void __cdecl free_bfeb1b(void* p) { g_freed = p; }

extern "C" void __fastcall _Dest_val_pair_ulong_CSmallVector_v2(DestVal* self);

int main()
{
    int dummy = 0;
    DestVal d;
    d.pad0 = (void*)0x11;
    d.buf  = (void*)&dummy;
    d.end  = (void*)0x33;

    _Dest_val_pair_ulong_CSmallVector_v2(&d);

    if (g_freed == (void*)&dummy && d.buf == 0 && d.end == 0 && d.pad0 == (void*)0x11)
        printf("006fb710_TEST PASS\n");
    else
        printf("006fb710_TEST FAIL freed=%p buf=%p end=%p\n", g_freed, d.buf, d.end);
    return 0;
}
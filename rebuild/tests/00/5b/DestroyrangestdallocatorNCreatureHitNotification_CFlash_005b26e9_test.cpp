#include <cstdio>

struct CFlash {
    void *vptr;
    void Dtor(int flags);
};
struct Alloc {};

static int g_calls = 0;
static int g_lastFlags = -1;

void CFlash::Dtor(int flags) {
    g_calls++;
    g_lastFlags = flags;
}

void __fastcall _Destroy_range_CFlash(CFlash *first, CFlash *last, Alloc *al)
{
    for (; first != last; ++first)
        first->Dtor(0);
}

int main() {
    CFlash arr[5];
    Alloc a;

    // empty range -> no calls
    _Destroy_range_CFlash(arr, arr, &a);
    if (g_calls != 0) { printf("FAIL empty\n"); return 1; }

    // full range -> 5 calls, flags always 0
    _Destroy_range_CFlash(arr, arr + 5, &a);
    if (g_calls != 5) { printf("FAIL count=%d\n", g_calls); return 1; }
    if (g_lastFlags != 0) { printf("FAIL flags=%d\n", g_lastFlags); return 1; }

    printf("DESTROY_RANGE_OK\n");
    return 0;
}
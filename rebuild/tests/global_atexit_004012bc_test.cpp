#include <cstdio>

typedef void (__cdecl *_PVFV)(void);

static _PVFV g_ret;
static _PVFV g_seen;

extern "C" _PVFV __cdecl _onexit_impl(_PVFV func)
{
    g_seen = func;
    return g_ret;
}

extern "C" int __cdecl atexit_impl(_PVFV func)
{
    return (_onexit_impl(func) == 0) ? -1 : 0;
}

static void dummy(void) {}

int main()
{
    g_ret = dummy;
    int ok = atexit_impl(dummy);
    if (ok != 0) { std::printf("FAIL: expected 0 on success, got %d\n", ok); return 1; }
    if (g_seen != dummy) { std::printf("FAIL: func not forwarded\n"); return 1; }

    g_ret = 0;
    int bad = atexit_impl(dummy);
    if (bad != -1) { std::printf("FAIL: expected -1 on failure, got %d\n", bad); return 1; }

    std::printf("_global_004012bc_TEST PASS\n");
    return 0;
}
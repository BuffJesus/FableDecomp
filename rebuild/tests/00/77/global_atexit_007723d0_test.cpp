#include "rebuild_abi.h"
#include <cstdio>
typedef void (*FableAtexitFn_007723d0)();
static int g_ret = 0; static void* g_fp = 0;
int FABLE_STDCALL FableAtexit_reg_007723d0(FableAtexitFn_007723d0 fp)
{ g_fp = (void*)fp; return g_ret; }
extern "C" int FABLE_STDCALL FableAtexit_007723d0(FableAtexitFn_007723d0 fp);
static void dummy() {}
int main()
{
    g_ret = 0; if (FableAtexit_007723d0(dummy) != 0) { std::printf("BAD0\n"); return 1; }
    g_ret = 7; if (FableAtexit_007723d0(dummy) != 1) { std::printf("BAD1\n"); return 1; }
    if (g_fp != (void*)dummy) { std::printf("BADFP\n"); return 1; }
    std::printf("ATEXIT_007723d0_OK\n");
    return 0;
}
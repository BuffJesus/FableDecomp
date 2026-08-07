#include "rebuild_abi.h"
#include <cstdio>
static void* g_args = 0;
struct FableSurfHelper_0082e4e0
{
    long compute(void* pargs);
};
long FableSurfHelper_0082e4e0::compute(void* pargs) { g_args = pargs; return 0x100; }
struct FableSurf_0082e4e0
{
    char pad[0x20];
    FableSurfHelper_0082e4e0 sub;
    long GetHeight(long a, long b);
};
int main()
{
    static FableSurf_0082e4e0 s;
    long r = s.GetHeight(1, 2);
    if (r != 0x100 + 0x18) { std::printf("BAD_RET\n"); return 1; }
    if (g_args == 0) { std::printf("BAD_ARGS\n"); return 1; }
    std::printf("GETHEIGHT_0082e4e0_OK\n");
    return 0;
}
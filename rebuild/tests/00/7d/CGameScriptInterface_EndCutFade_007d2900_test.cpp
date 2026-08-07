#include "rebuild_abi.h"
#include <cstdio>
static int g_h1 = 0, g_h2 = 0, g_v = 0;
struct FableEndCut_007d2900
{
    void FABLE_FASTCALL h1();
    void h2(int v);
    void EndCutFade();
};
void FABLE_FASTCALL FableEndCut_007d2900::h1() { g_h1++; }
void FableEndCut_007d2900::h2(int v) { g_h2++; g_v = v; }
int main()
{
    FableEndCut_007d2900 c;
    c.EndCutFade();
    if (g_h1 != 1 || g_h2 != 1 || g_v != 7) { std::printf("BAD\n"); return 1; }
    std::printf("ENDCUT_007d2900_OK\n");
    return 0;
}
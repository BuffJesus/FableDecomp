#include "rebuild_abi.h"
#include <cstdio>
struct FableUmoveElem_0059387c;
static void* g_p = 0; static char* g_local = 0;
void FABLE_STDCALL FableUmove_construct_0059387c(FableUmoveElem_0059387c* p, char* local)
{
    g_p = (void*)p; g_local = local;
}
extern "C" void FABLE_STDCALL FableUmove_0059387c(FableUmoveElem_0059387c* p);
int main()
{
    int marker = 0;
    FableUmove_0059387c((FableUmoveElem_0059387c*)&marker);
    if (g_p != (void*)&marker) { std::printf("BAD_P\n"); return 1; }
    if (g_local == 0) { std::printf("BAD_LOCAL\n"); return 1; }
    std::printf("UMOVE_0059387c_OK\n");
    return 0;
}
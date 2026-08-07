#include "rebuild_abi.h"
#include <cstdio>
struct FableUmoveElem_0065e5a4;
static void* g_p = 0; static char* g_local = 0;
void FABLE_STDCALL FableUmove_construct_0065e5a4(FableUmoveElem_0065e5a4* p, char* local)
{ g_p = (void*)p; g_local = local; }
extern "C" void FABLE_STDCALL FableUmove_0065e5a4(FableUmoveElem_0065e5a4* p);
int main()
{
    int marker = 0;
    FableUmove_0065e5a4((FableUmoveElem_0065e5a4*)&marker);
    if (g_p != (void*)&marker) { std::printf("BAD_P\n"); return 1; }
    if (g_local == 0) { std::printf("BAD_LOCAL\n"); return 1; }
    std::printf("UMOVE_0065e5a4_OK\n");
    return 0;
}
#include "rebuild_abi.h"
#include <cstdio>
struct FableUmoveElem_0065571a;
static void* g_p = 0; static char* g_local = 0;
void FABLE_STDCALL FableUmove_construct_0065571a(FableUmoveElem_0065571a* p, char* local)
{ g_p = (void*)p; g_local = local; }
extern "C" void FABLE_STDCALL FableUmove_0065571a(FableUmoveElem_0065571a* p);
int main()
{
    int marker = 0;
    FableUmove_0065571a((FableUmoveElem_0065571a*)&marker);
    if (g_p != (void*)&marker) { std::printf("BAD_P\n"); return 1; }
    if (g_local == 0) { std::printf("BAD_LOCAL\n"); return 1; }
    std::printf("UMOVE_0065571a_OK\n");
    return 0;
}
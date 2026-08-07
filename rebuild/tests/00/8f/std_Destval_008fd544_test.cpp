#include "rebuild_abi.h"
#include <cstdio>
struct FableDestVal_008fd544_t
{ char pad[0x14]; void FABLE_FASTCALL inner(); void FABLE_FASTCALL outer(); };
static int g_inner = 0, g_outer = 0;
void FABLE_FASTCALL FableDestVal_008fd544_t::inner() { g_inner++; }
void FABLE_FASTCALL FableDestVal_008fd544_t::outer() { g_outer++; }
extern "C" void FABLE_FASTCALL FableDestVal_008fd544(FableDestVal_008fd544_t* self);
int main()
{
    char buf[0x14 + 8];
    FableDestVal_008fd544((FableDestVal_008fd544_t*)buf);
    if (g_inner != 1) { std::printf("BAD_INNER\n"); return 1; }
    if (g_outer != 1) { std::printf("BAD_OUTER\n"); return 1; }
    std::printf("DESTVAL_008fd544_OK\n"); return 0;
}
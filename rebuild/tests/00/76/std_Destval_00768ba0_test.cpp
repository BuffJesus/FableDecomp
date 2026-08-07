#include "rebuild_abi.h"
#include <cstdio>
struct FableDestVal_00768ba0_t
{
    char pad[0x4];
    void FABLE_FASTCALL inner();
    void FABLE_FASTCALL outer();
};
static int g_inner = 0, g_outer = 0;
void FABLE_FASTCALL FableDestVal_00768ba0_t::inner() { g_inner++; }
void FABLE_FASTCALL FableDestVal_00768ba0_t::outer() { g_outer++; }
extern "C" void FABLE_FASTCALL FableDestVal_00768ba0(FableDestVal_00768ba0_t* self);
int main()
{
    char buf[0x4 + 8];
    FableDestVal_00768ba0((FableDestVal_00768ba0_t*)buf);
    if (g_inner != 1) { std::printf("BAD_INNER\n"); return 1; }
    if (g_outer != 1) { std::printf("BAD_OUTER\n"); return 1; }
    std::printf("DESTVAL_00768ba0_OK\n");
    return 0;
}
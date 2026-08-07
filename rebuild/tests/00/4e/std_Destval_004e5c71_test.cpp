#include "rebuild_abi.h"
#include <cstdio>
struct FableDestVal_004e5c71_t
{
    char pad[0x18];
    void FABLE_FASTCALL inner();
    void FABLE_FASTCALL outer();
};
static int g_inner = 0, g_outer = 0;
void FABLE_FASTCALL FableDestVal_004e5c71_t::inner() { g_inner++; }
void FABLE_FASTCALL FableDestVal_004e5c71_t::outer() { g_outer++; }
extern "C" void FABLE_FASTCALL FableDestVal_004e5c71(FableDestVal_004e5c71_t* self);
int main()
{
    char buf[0x18 + 8];
    FableDestVal_004e5c71_t* self = (FableDestVal_004e5c71_t*)buf;
    FableDestVal_004e5c71(self);
    if (g_inner != 1) { std::printf("BAD_INNER\n"); return 1; }
    if (g_outer != 1) { std::printf("BAD_OUTER\n"); return 1; }
    std::printf("DESTVAL_004e5c71_OK\n");
    return 0;
}
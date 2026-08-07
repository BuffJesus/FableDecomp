#include "rebuild_abi.h"
#include <cstdio>
static int g_h1 = 0, g_h2 = 0; static char g_flag = 9; static void* g_this = 0;
struct FableDestValV_008d6a30_t
{
    void* field_0;
    void* field_4;
    void FABLE_FASTCALL h1();
};
struct FableDestValV_008d6a30_inner
{
    void FABLE_FASTCALL h2(char flag);
};
void FABLE_FASTCALL FableDestValV_008d6a30_t::h1() { g_h1++; }
void FABLE_FASTCALL FableDestValV_008d6a30_inner::h2(char flag) { g_h2++; g_flag = flag; g_this = (void*)this; }
extern "C" void FABLE_FASTCALL FableDestValV_008d6a30(FableDestValV_008d6a30_t* self);
int main()
{
    static FableDestValV_008d6a30_inner inner;
    static FableDestValV_008d6a30_t s;
    s.field_0 = 0; s.field_4 = &inner;
    FableDestValV_008d6a30(&s);
    if (g_h1 != 1 || g_h2 != 1) { std::printf("BAD_CALL\n"); return 1; }
    if (g_flag != 0) { std::printf("BAD_FLAG\n"); return 1; }
    if (g_this != (void*)&inner) { std::printf("BAD_THIS\n"); return 1; }
    std::printf("DESTVALV_008d6a30_OK\n"); return 0;
}
#include <cstdio>

struct F4;
struct F4Vtbl { void (__fastcall *slot[8])(F4*); };
struct F4 { F4Vtbl* vtbl; };
struct CQ_SunnyvaleMasterScript { void* f0; F4* f4; };

extern "C" char __fastcall helper_f35b30(CQ_SunnyvaleMasterScript* self);

void __fastcall Main(CQ_SunnyvaleMasterScript* self);

static int g_slot7_calls = 0;
static int g_helper_calls = 0;

void __fastcall slot7_impl(F4* /*p*/) { g_slot7_calls++; }

extern "C" char __fastcall helper_f35b30(CQ_SunnyvaleMasterScript* /*self*/)
{
    g_helper_calls++;
    // loop-while-zero: return zero for first 2 iters, nonzero on 3rd to exit
    return (char)(g_helper_calls < 3 ? 0 : 1);
}

int main()
{
    F4Vtbl vt;
    for (int i = 0; i < 8; i++) vt.slot[i] = 0;
    vt.slot[7] = slot7_impl;
    F4 f4; f4.vtbl = &vt;
    CQ_SunnyvaleMasterScript obj; obj.f0 = 0; obj.f4 = &f4;

    Main(&obj);

    if (g_slot7_calls == 3 && g_helper_calls == 3)
        printf("00dc44e0_TEST PASS\n");
    else
        printf("FAIL slot7=%d helper=%d\n", g_slot7_calls, g_helper_calls);
    return 0;
}
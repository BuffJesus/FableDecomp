#include <cstdio>

struct Sub;
struct CQ_SunnyvaleMasterScript;

struct SubVtbl {
    void* slot0; void* slot1; void* slot2; void* slot3;
    void* slot4; void* slot5; void* slot6;
    void (__fastcall *slot7)(Sub*);
};
struct Sub { SubVtbl* vtbl; };
struct CQ_SunnyvaleMasterScript { void* f0; Sub* f4; };

static int g_vcalls = 0;
static int g_helpercalls = 0;

void __fastcall slot7_impl(Sub*) { ++g_vcalls; }

// helper returns false twice, then true -> loop body runs 3 times
extern "C" bool __fastcall Q_Sunnyvale_helper_f35b30(CQ_SunnyvaleMasterScript*)
{
    ++g_helpercalls;
    return g_helpercalls >= 3;
}

void __fastcall CQ_SunnyvaleMasterScript__Main(CQ_SunnyvaleMasterScript* self);

int main()
{
    SubVtbl vtab[1]; vtab[0].slot7 = slot7_impl;
    Sub subinst[1]; subinst[0].vtbl = vtab;
    CQ_SunnyvaleMasterScript objinst[1]; objinst[0].f0 = 0; objinst[0].f4 = subinst;

    CQ_SunnyvaleMasterScript__Main(objinst);

    // do-while: body+helper each run until helper true on 3rd call
    if (g_vcalls == 3 && g_helpercalls == 3)
        std::printf("00dc84a0_TEST PASS\n");
    else
        std::printf("FAIL v=%d h=%d\n", g_vcalls, g_helpercalls);
    return 0;
}
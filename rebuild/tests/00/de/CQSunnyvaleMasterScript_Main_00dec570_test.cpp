#include <cstdio>

struct Sub;

struct SubVtbl {
    void (__fastcall *slot0)(Sub*);
    void (__fastcall *slot1)(Sub*);
    void (__fastcall *slot2)(Sub*);
    void (__fastcall *slot3)(Sub*);
    void (__fastcall *slot4)(Sub*);
    void (__fastcall *slot5)(Sub*);
    void (__fastcall *slot6)(Sub*);
    void (__fastcall *slot7)(Sub*);
};

struct Sub {
    SubVtbl* vtbl;
};

struct CQ_SunnyvaleMasterScript {
    void* f0;
    Sub*  f4;
};

extern "C" bool __fastcall CQ_SunnyvaleMasterScript_helper(CQ_SunnyvaleMasterScript* self);

void __fastcall CQ_SunnyvaleMasterScript__Main(CQ_SunnyvaleMasterScript* self);

static int g_slotCalls = 0;
static int g_helperCalls = 0;

void __fastcall test_slot7(Sub*) { ++g_slotCalls; }

extern "C" bool __fastcall CQ_SunnyvaleMasterScript_helper(CQ_SunnyvaleMasterScript*)
{
    ++g_helperCalls;
    // return false twice, then true -> 3 iterations
    return g_helperCalls >= 3;
}

int main()
{
    SubVtbl vt = {0,0,0,0,0,0,0, test_slot7};
    Sub theSub; theSub.vtbl = &vt;
    CQ_SunnyvaleMasterScript obj; obj.f0 = 0; obj.f4 = &theSub;

    CQ_SunnyvaleMasterScript__Main(&obj);

    if (g_slotCalls == 3 && g_helperCalls == 3)
        printf("00dec570_TEST PASS\n");
    else
        printf("FAIL slot=%d helper=%d\n", g_slotCalls, g_helperCalls);
    return 0;
}
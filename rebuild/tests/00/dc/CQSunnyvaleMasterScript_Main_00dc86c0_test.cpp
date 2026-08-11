#include <stdio.h>

struct CObjWithVtbl;

struct CObjVtbl {
    void (__fastcall *slot0)(CObjWithVtbl*);
    void (__fastcall *slot1)(CObjWithVtbl*);
    void (__fastcall *slot2)(CObjWithVtbl*);
    void (__fastcall *slot3)(CObjWithVtbl*);
    void (__fastcall *slot4)(CObjWithVtbl*);
    void (__fastcall *slot5)(CObjWithVtbl*);
    void (__fastcall *slot6)(CObjWithVtbl*);
    void (__fastcall *slot7)(CObjWithVtbl*);
};

struct CObjWithVtbl {
    CObjVtbl* vtbl;
};

struct CQ_SunnyvaleMasterScript {
    void*         f0;
    CObjWithVtbl* f4;
};

static int g_slot7_calls = 0;
static int g_helper_calls = 0;

void __fastcall test_slot7(CObjWithVtbl*) { g_slot7_calls++; }

extern "C" char __fastcall SunnyvaleMasterHelper(CQ_SunnyvaleMasterScript* self)
{
    g_helper_calls++;
    // return nonzero on 3rd call -> loop runs 3 times
    return (g_helper_calls >= 3) ? (char)1 : (char)0;
}

void __fastcall CQ_SunnyvaleMasterScript_Main(CQ_SunnyvaleMasterScript* self);

int main()
{
    CObjVtbl vt;
    vt.slot7 = test_slot7;
    CObjWithVtbl obj;
    obj.vtbl = &vt;
    CQ_SunnyvaleMasterScript s;
    s.f0 = 0;
    s.f4 = &obj;

    CQ_SunnyvaleMasterScript_Main(&s);

    if (g_slot7_calls == 3 && g_helper_calls == 3)
        printf("00dc86c0_TEST PASS\n");
    else
        printf("FAIL slot7=%d helper=%d\n", g_slot7_calls, g_helper_calls);
    return 0;
}
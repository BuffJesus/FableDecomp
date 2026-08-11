#include <cstdio>

struct IVObj {
    struct VT { void (__fastcall *slot0)(IVObj*); void (__fastcall *slot1)(IVObj*);
        void (__fastcall *slot2)(IVObj*); void (__fastcall *slot3)(IVObj*);
        void (__fastcall *slot4)(IVObj*); void (__fastcall *slot5)(IVObj*);
        void (__fastcall *slot6)(IVObj*); void (__fastcall *slot7_1c)(IVObj*); };
    VT* vt;
};

struct CQ_SunnyvaleMasterScript {
    void* f0;
    IVObj* f4;
};

extern "C" bool __fastcall SunnyvaleHelper_f35b30(CQ_SunnyvaleMasterScript* self);

void __fastcall CQ_SunnyvaleMasterScript_Main(CQ_SunnyvaleMasterScript* self);

static int g_vcalls = 0;
static int g_helper_calls = 0;

static void __fastcall fake_slot(IVObj*) { g_vcalls++; }

extern "C" bool __fastcall SunnyvaleHelper_f35b30(CQ_SunnyvaleMasterScript*) {
    g_helper_calls++;
    return (g_helper_calls >= 3); // false, false, true -> loop exits on true
}

int main() {
    IVObj::VT vt = {0,0,0,0,0,0,0,fake_slot};
    IVObj obj; obj.vt = &vt;
    CQ_SunnyvaleMasterScript self; self.f0 = 0; self.f4 = &obj;

    CQ_SunnyvaleMasterScript_Main(&self);

    // do-while: body runs, then helper; loops while helper true.
    // helper returns true,true,false => body runs 3 times, helper called 3 times.
    if (g_vcalls == 3 && g_helper_calls == 3)
        printf("00e192a0_TEST PASS\n");
    else
        printf("FAIL vcalls=%d helper=%d\n", g_vcalls, g_helper_calls);
    return 0;
}
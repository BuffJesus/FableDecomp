#include <stdio.h>

struct F4 {
    struct VT {
        void (__fastcall *slot0)(F4*);
        void (__fastcall *slot1)(F4*);
        void (__fastcall *slot2)(F4*);
        void (__fastcall *slot3)(F4*);
        void (__fastcall *slot4)(F4*);
        void (__fastcall *slot5)(F4*);
        void (__fastcall *slot6)(F4*);
        void (__fastcall *slot7)(F4*);
    };
    VT* vtbl;
};

struct CQ_SunnyvaleMasterScript {
    void* pad;
    F4*   f4;
};

static int g_ticks = 0;
static int g_limit = 3;

static void __fastcall tick(F4*) { g_ticks++; }

extern "C" bool __fastcall helper_f35b30(CQ_SunnyvaleMasterScript*) {
    // loop exits (do-while ends) when this returns true; keep looping while false
    return g_ticks >= g_limit;
}

void __fastcall Main(CQ_SunnyvaleMasterScript* self);

int main() {
    F4::VT vt;
    vt.slot7 = tick;
    F4 f; f.vtbl = &vt;
    CQ_SunnyvaleMasterScript o; o.pad = 0; o.f4 = &f;

    Main(&o);

    // do-while: runs slot7 at least once, then loops while helper true.
    // g_limit=3 => ticks: 1(ticks=1,cont),2(cont),3(stop) => 3 ticks.
    if (g_ticks == 3) {
        printf("00ce0350_TEST PASS\n");
    } else {
        printf("FAIL ticks=%d\n", g_ticks);
    }
    return 0;
}
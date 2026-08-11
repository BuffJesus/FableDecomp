#include <cstdio>

struct IThing;
struct IThingVtbl {
    void* slot00; void* slot04; void* slot08; void* slot0c;
    void* slot10; void* slot14; void* slot18;
    char (__fastcall *slot1c)(IThing* self);
};
struct IThing { IThingVtbl* vtbl; };
struct CQ_SunnyvaleMasterScript { void* field0; IThing* field4; };

static int g_vcalls = 0;
static char __fastcall thing_slot1c(IThing*) { g_vcalls++; return 0; }

static int g_helper_calls = 0;
extern "C" char __fastcall SunnyvaleMasterHelper(CQ_SunnyvaleMasterScript*) {
    g_helper_calls++;
    // return nonzero on 3rd call so loop runs 3 iterations
    return (g_helper_calls >= 3) ? 1 : 0;
}

void __fastcall CQ_SunnyvaleMasterScript__Main(CQ_SunnyvaleMasterScript* self);

int main() {
    IThingVtbl vt; vt.slot1c = &thing_slot1c;
    IThing thing; thing.vtbl = &vt;
    CQ_SunnyvaleMasterScript obj; obj.field0 = 0; obj.field4 = &thing;

    CQ_SunnyvaleMasterScript__Main(&obj);

    if (g_vcalls == 3 && g_helper_calls == 3)
        printf("00e191a0_TEST PASS\n");
    else
        printf("FAIL vcalls=%d helper=%d\n", g_vcalls, g_helper_calls);
    return 0;
}
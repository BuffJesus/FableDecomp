#include <cstdio>

struct CCreatureAction_ForcePushOutOf;

struct CCreatureAction_ForcePushOutOf_vtbl {
    void (__fastcall *slot0)(CCreatureAction_ForcePushOutOf*);
    void (__fastcall *slot1)(CCreatureAction_ForcePushOutOf*);
    void (__fastcall *slot2)(CCreatureAction_ForcePushOutOf*);
    void (__fastcall *slot3)(CCreatureAction_ForcePushOutOf*);
};

struct CCreatureAction_ForcePushOutOf {
    CCreatureAction_ForcePushOutOf_vtbl* vtbl;
    int marker;
};

static int g_checkResult = 0;
static int g_slot3Called = 0;
static int g_baseCalled = 0;

bool __fastcall CreatureForcePush_Check(CCreatureAction_ForcePushOutOf* self) {
    (void)self;
    return g_checkResult != 0;
}
void __fastcall CreatureAction_FinishAction_Base(CCreatureAction_ForcePushOutOf* self) {
    (void)self;
    g_baseCalled++;
}
static void __fastcall Slot3Impl(CCreatureAction_ForcePushOutOf* self) {
    self->marker = 99;
    g_slot3Called++;
}

void __fastcall CCreatureAction_ForcePushOutOf_FinishAction(CCreatureAction_ForcePushOutOf* self);

int main() {
    CCreatureAction_ForcePushOutOf_vtbl vt;
    vt.slot0 = vt.slot1 = vt.slot2 = 0;
    vt.slot3 = Slot3Impl;

    CCreatureAction_ForcePushOutOf obj;
    obj.vtbl = &vt;
    obj.marker = 0;

    // Case 1: check returns false -> slot3 called, base called
    g_checkResult = 0; g_slot3Called = 0; g_baseCalled = 0;
    CCreatureAction_ForcePushOutOf_FinishAction(&obj);
    bool ok1 = (g_slot3Called == 1) && (g_baseCalled == 1) && (obj.marker == 99);

    // Case 2: check returns true -> slot3 skipped, base still called
    g_checkResult = 1; g_slot3Called = 0; g_baseCalled = 0; obj.marker = 0;
    CCreatureAction_ForcePushOutOf_FinishAction(&obj);
    bool ok2 = (g_slot3Called == 0) && (g_baseCalled == 1) && (obj.marker == 0);

    if (ok1 && ok2) {
        printf("FORCEPUSH_FINISH_OK\n");
        return 0;
    }
    printf("FAIL c1=%d c2=%d\n", (int)ok1, (int)ok2);
    return 1;
}
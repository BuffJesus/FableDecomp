#include <cstdio>

// Standalone byte-exactness behavior harness for
// CGameScriptInterface::EntityResetCombatTypeToDefault @ 0x00890f40

struct Entity;
struct ArgObj;
struct DefObj;

struct ArgVtbl {
    void* pad[0x2c / 4];
    Entity* (__fastcall* GetTargetEntity)(void* self);
};
struct ArgObj {
    ArgVtbl* vtbl;
};

struct Entity {
    unsigned char pad00[0x6c];
    unsigned char m_Flags6c;
    unsigned char pad6d[0x91 - 0x6d];
    unsigned char m_Flags91;
    int  Helper_ffdd1230();
    DefObj* Helper_ffc36a50(void* pLocalA, int a);
};

struct DefObj {
    void** Helper_11bd80();
};

struct GObj {
    void Helper_143a70(void* pLocalB, void* value);
};

struct Local10 {
    void* m_data;
    void Dtor_10dba0();
};

struct SelfVtbl;
struct SelfObj {
    SelfVtbl* vtbl;
};
struct SelfVtbl {
    unsigned char pad[0x734];
    void (SelfObj::* ResetCombatType)(ArgObj* arg, void* pLocal);
};

// ---- recording state ----
static int   g_calls_ffdd1230 = 0;
static int   g_calls_ffc36a50 = 0;
static int   g_calls_11bd80   = 0;
static int   g_calls_143a70   = 0;
static int   g_calls_reset    = 0;
static int   g_calls_dtor     = 0;

static void* g_ffc36a50_pLocalA = 0;
static int   g_ffc36a50_a       = -1;
static void* g_143a70_pLocalB   = 0;
static void* g_143a70_value     = 0;
static ArgObj* g_reset_arg      = 0;
static void* g_reset_pLocal     = 0;
static void* g_dtor_this        = 0;

static void*  g_defPtr_storage;
static void** g_defPtr = &g_defPtr_storage;
static DefObj g_defObj_inst;

GObj g_obj13ca828;

int Entity::Helper_ffdd1230() {
    ++g_calls_ffdd1230;
    return 0x1234;
}
DefObj* Entity::Helper_ffc36a50(void* pLocalA, int a) {
    ++g_calls_ffc36a50;
    g_ffc36a50_pLocalA = pLocalA;
    g_ffc36a50_a = a;
    return &g_defObj_inst;
}
void** DefObj::Helper_11bd80() {
    ++g_calls_11bd80;
    g_defPtr_storage = (void*)0xABCDEF00;
    return g_defPtr;
}
void GObj::Helper_143a70(void* pLocalB, void* value) {
    ++g_calls_143a70;
    g_143a70_pLocalB = pLocalB;
    g_143a70_value = value;
}
void Local10::Dtor_10dba0() {
    ++g_calls_dtor;
    g_dtor_this = this;
}

static void ResetCombatType_impl(SelfObj* self, ArgObj* arg, void* pLocal) {
    ++g_calls_reset;
    g_reset_arg = arg;
    g_reset_pLocal = pLocal;
}
struct SelfObjTramp : SelfObj {
    void Reset(ArgObj* a, void* p) { ResetCombatType_impl(this, a, p); }
};

// ---- the function under test (byte-exact source) ----
void __fastcall CGameScriptInterface_EntityResetCombatTypeToDefault(
    void* self_ecx, unsigned long /*edx*/, ArgObj* arg)
{
    SelfObj* self = (SelfObj*)self_ecx;

    Entity* entity = arg->vtbl->GetTargetEntity(arg);
    if (entity == 0)
        return;
    if (entity->m_Flags91 & 0x01)
        return;
    if ((entity->m_Flags6c & 0x08) == 0)
        return;

    void* localA;
    Local10 localB;

    int a = entity->Helper_ffdd1230();
    DefObj* defObj = entity->Helper_ffc36a50(&localA, a);
    void** defPtr = defObj->Helper_11bd80();
    void* value = *defPtr;
    g_obj13ca828.Helper_143a70(&localB, value);

    (self->*(self->vtbl->ResetCombatType))(arg, &localB);

    localB.Dtor_10dba0();
}

// ---- harness plumbing ----
static Entity* g_entity_to_return;
static Entity* __fastcall GetTargetEntity_impl(void* /*self*/) {
    return g_entity_to_return;
}

static void reset_counters() {
    g_calls_ffdd1230 = g_calls_ffc36a50 = g_calls_11bd80 = 0;
    g_calls_143a70 = g_calls_reset = g_calls_dtor = 0;
    g_ffc36a50_pLocalA = 0; g_ffc36a50_a = -1;
    g_143a70_pLocalB = 0; g_143a70_value = 0;
    g_reset_arg = 0; g_reset_pLocal = 0; g_dtor_this = 0;
}

int main() {
    ArgVtbl avt;
    for (int i = 0; i < 0x2c / 4; ++i) avt.pad[i] = 0;
    avt.GetTargetEntity = GetTargetEntity_impl;
    ArgObj arg;
    arg.vtbl = &avt;

    SelfVtbl svt;
    for (int i = 0; i < 0x734; ++i) svt.pad[i] = 0;
    svt.ResetCombatType =
        (void (SelfObj::*)(ArgObj*, void*)) &SelfObjTramp::Reset;
    SelfObjTramp selfObj;
    selfObj.vtbl = &svt;

    Entity ent;

    int failed = 0;

    // Case 1: entity == 0 -> early return
    reset_counters();
    g_entity_to_return = 0;
    CGameScriptInterface_EntityResetCombatTypeToDefault(&selfObj, 0, &arg);
    if (g_calls_ffdd1230 != 0 || g_calls_reset != 0 || g_calls_dtor != 0) {
        printf("FAIL case1: ran body on null entity\n"); failed = 1;
    }

    // Case 2: flag91 bit0 set -> early return
    reset_counters();
    ent.m_Flags91 = 0x01;
    ent.m_Flags6c = 0x08;
    g_entity_to_return = &ent;
    CGameScriptInterface_EntityResetCombatTypeToDefault(&selfObj, 0, &arg);
    if (g_calls_ffdd1230 != 0 || g_calls_reset != 0) {
        printf("FAIL case2: ran body with flag91 set\n"); failed = 1;
    }

    // Case 3: flag6c bit3 clear -> early return
    reset_counters();
    ent.m_Flags91 = 0x00;
    ent.m_Flags6c = 0x00;
    g_entity_to_return = &ent;
    CGameScriptInterface_EntityResetCombatTypeToDefault(&selfObj, 0, &arg);
    if (g_calls_ffdd1230 != 0 || g_calls_reset != 0) {
        printf("FAIL case3: ran body with flag6c clear\n"); failed = 1;
    }

    // Case 4: full path
    reset_counters();
    ent.m_Flags91 = 0x00;
    ent.m_Flags6c = 0x08;
    g_entity_to_return = &ent;
    CGameScriptInterface_EntityResetCombatTypeToDefault(&selfObj, 0, &arg);

    if (g_calls_ffdd1230 != 1) { printf("FAIL case4: ffdd1230 count\n"); failed = 1; }
    if (g_calls_ffc36a50 != 1) { printf("FAIL case4: ffc36a50 count\n"); failed = 1; }
    if (g_calls_11bd80   != 1) { printf("FAIL case4: 11bd80 count\n");   failed = 1; }
    if (g_calls_143a70   != 1) { printf("FAIL case4: 143a70 count\n");   failed = 1; }
    if (g_calls_reset    != 1) { printf("FAIL case4: reset count\n");    failed = 1; }
    if (g_calls_dtor     != 1) { printf("FAIL case4: dtor count\n");     failed = 1; }
    if (g_ffc36a50_a != 0x1234) { printf("FAIL case4: 'a' not threaded\n"); failed = 1; }
    if (g_143a70_value != (void*)0xABCDEF00) { printf("FAIL case4: value not *defPtr\n"); failed = 1; }
    if (g_143a70_pLocalB != g_reset_pLocal) { printf("FAIL case4: reset localB mismatch\n"); failed = 1; }
    if (g_143a70_pLocalB != g_dtor_this)    { printf("FAIL case4: dtor localB mismatch\n"); failed = 1; }
    if (g_reset_arg != &arg) { printf("FAIL case4: arg not forwarded\n"); failed = 1; }
    if (g_ffc36a50_pLocalA == g_143a70_pLocalB) { printf("FAIL case4: localA aliased localB\n"); failed = 1; }

    if (failed) { printf("TEST FAILED\n"); return 1; }

    printf("FSE2_00890f40_TEST PASS\n");
    return 0;
}
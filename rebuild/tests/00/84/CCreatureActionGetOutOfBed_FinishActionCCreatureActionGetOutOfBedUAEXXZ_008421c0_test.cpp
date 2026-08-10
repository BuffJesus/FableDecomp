#include <stdio.h>

struct CCreatureVtbl {
    void (__fastcall *slot0)(void*);
    void (__fastcall *slot4)(void*);
    void (__fastcall *slot8)(void*);
    void (__fastcall *slot12)(void*);
};

struct CCreatureAction_GetOutOfBed {
    CCreatureVtbl* vtbl;
    char pad[0x62 - 4];
    bool interrupted;
};

static int g_virt = 0;
static int g_base = 0;

static void __fastcall my_slot12(void* p) { g_virt++; }
static CCreatureVtbl g_vtbl = { 0, 0, 0, my_slot12 };

void __fastcall CCreatureAction_Base_FinishAction(CCreatureAction_GetOutOfBed* self) { g_base++; }

void __fastcall FinishAction(CCreatureAction_GetOutOfBed* self);

int main() {
    CCreatureAction_GetOutOfBed obj;
    obj.vtbl = &g_vtbl;

    obj.interrupted = false;
    FinishAction(&obj);
    if (g_virt != 1 || g_base != 1) { printf("FAIL a\n"); return 1; }

    obj.interrupted = true;
    FinishAction(&obj);
    if (g_virt != 1 || g_base != 2) { printf("FAIL b\n"); return 1; }

    printf("OK_008421c0\n");
    return 0;
}
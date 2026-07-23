// Standalone behaviour test for CThingCreatureBase::GetCombatManager @ 0x00661ed0
#include <stdio.h>

struct CThingCreatureBase;
struct CCombatManager;

extern void * __fastcall CThingCreatureBase_GetCombatBase(CThingCreatureBase *self);

CCombatManager * __fastcall CThingCreatureBase_GetCombatManager(CThingCreatureBase *self)
{
    unsigned char *base = (unsigned char *)CThingCreatureBase_GetCombatBase(self);
    return *(CCombatManager **)(base + 0x4c);
}

// ---- test fixture ----
// The helper returns a base object; at +0x4c it stores the combat manager ptr.
static unsigned char g_base[0x50 + 4];

void * __fastcall CThingCreatureBase_GetCombatBase(CThingCreatureBase *self)
{
    (void)self;
    return g_base;
}

int main(void)
{
    CCombatManager *sentinel = (CCombatManager *)0xDEADBEEF;
    *(CCombatManager **)(g_base + 0x4c) = sentinel;

    CCombatManager *got = CThingCreatureBase_GetCombatManager((CThingCreatureBase *)0x12340000);

    if (got == sentinel)
        printf("CTHINGCREATUREBASE_00661ed0_TEST PASS\n");
    else
        printf("CTHINGCREATUREBASE_00661ed0_TEST FAIL got=%p want=%p\n", (void*)got, (void*)sentinel);
    return 0;
}
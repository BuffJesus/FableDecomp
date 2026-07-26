// Standalone behaviour test for CThingCreatureBase::GetCombatManager @ 0x00661ec0
// Compiles standalone; provides a trivial stub for the relocation-masked helper.
#include <cstdio>

struct CCombatManager;

struct CThingCreatureHelperResult
{
    char _pad[0x4c];
    CCombatManager *pCombatManager; // +0x4c
};

struct CThingCreatureBase
{
    char _opaque[4];
};

// Stub helper: returns a fixture object whose +0x4c holds our sentinel manager.
static CThingCreatureHelperResult g_fixture;

CThingCreatureHelperResult * __fastcall
CThingCreatureBase_GetCombatManagerHelper(void *pThis)
{
    (void)pThis;
    return &g_fixture;
}

// The function under test lives in the reconstruction object.
CCombatManager * __fastcall CThingCreatureBase__GetCombatManager(CThingCreatureBase *pThis);

int main()
{
    CCombatManager *sentinel = (CCombatManager *)0xDEADBEEF;
    g_fixture.pCombatManager = sentinel;

    CThingCreatureBase obj;
    CCombatManager *got = CThingCreatureBase__GetCombatManager(&obj);

    if (got == sentinel)
        std::printf("CTHINGCREATUREBASE_00661ec0_TEST PASS\n");
    else
        std::printf("CTHINGCREATUREBASE_00661ec0_TEST FAIL got=%p\n", (void *)got);
    return 0;
}

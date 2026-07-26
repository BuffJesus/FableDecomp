// Standalone behaviour test for CTCVillage::GetIsHeroEnemyOfGuards @ 0x00658604
// cl test.cpp && test.exe  -> prints "CTCVILLAGE_00658604_TEST PASS"
#include <stdio.h>

// Object layout: field at +0xd4 is the compared threshold.
struct CTCVillage
{
    char _pad0[0xd4];
    int  m_guardEnemyThreshold; // +0xd4
};

// Controllable stub returns for the two engine callees.
static int g_prepReturn;
static int g_queryReturn;

extern "C" int __cdecl CTCVillage_GetIsHeroEnemyOfGuards_Prep(void)
{
    return g_prepReturn; // discarded by the accessor; just must link
}
extern "C" int __cdecl CTCVillage_GetIsHeroEnemyOfGuards_Query(void)
{
    return g_queryReturn;
}

// Same body as the ported accessor (extern "C" stubs above).
extern int __cdecl CTCVillage_GetIsHeroEnemyOfGuards_Prep(void);
extern int __cdecl CTCVillage_GetIsHeroEnemyOfGuards_Query(void);

bool __fastcall CTCVillage_GetIsHeroEnemyOfGuards(CTCVillage* thisPtr)
{
    CTCVillage_GetIsHeroEnemyOfGuards_Prep();
    return CTCVillage_GetIsHeroEnemyOfGuards_Query() < thisPtr->m_guardEnemyThreshold;
}

int main(void)
{
    CTCVillage v;
    for (int i = 0; i < (int)sizeof(v); ++i) ((char*)&v)[i] = 0;

    int fails = 0;

    // Case 1: Query() < field  -> true
    v.m_guardEnemyThreshold = 10;
    g_prepReturn = 999; g_queryReturn = 3;
    if (CTCVillage_GetIsHeroEnemyOfGuards(&v) != true) { printf("FAIL c1\n"); ++fails; }

    // Case 2: Query() == field -> false (setl is strict less-than)
    g_queryReturn = 10;
    if (CTCVillage_GetIsHeroEnemyOfGuards(&v) != false) { printf("FAIL c2\n"); ++fails; }

    // Case 3: Query() > field  -> false
    g_queryReturn = 25;
    if (CTCVillage_GetIsHeroEnemyOfGuards(&v) != false) { printf("FAIL c3\n"); ++fails; }

    // Case 4: signed comparison (negative Query, positive field) -> true
    v.m_guardEnemyThreshold = 5;
    g_queryReturn = -7;
    if (CTCVillage_GetIsHeroEnemyOfGuards(&v) != true) { printf("FAIL c4\n"); ++fails; }

    // Case 5: Prep() return must not affect result
    g_prepReturn = -12345; g_queryReturn = 100; v.m_guardEnemyThreshold = 200;
    if (CTCVillage_GetIsHeroEnemyOfGuards(&v) != true) { printf("FAIL c5\n"); ++fails; }

    if (fails == 0) printf("CTCVILLAGE_00658604_TEST PASS\n");
    else printf("CTCVILLAGE_00658604_TEST FAIL (%d)\n", fails);
    return fails;
}
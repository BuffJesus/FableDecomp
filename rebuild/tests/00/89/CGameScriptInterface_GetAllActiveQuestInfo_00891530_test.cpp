#include <cstdio>

// Standalone behavior fixture for
// CGameScriptInterface::GetAllActiveQuestInfo @ 0x00891530.
struct CCharStringVector
{
    int marker;
};

struct CQuestManager
{
    void GetAllActiveQuestInfo(
        CCharStringVector& questNames,
        CCharStringVector& questInfo);
};

static void* g_managerThis;
static CCharStringVector* g_namesArgument;
static CCharStringVector* g_infoArgument;
static int g_callCount;

CQuestManager* DAT_013b89fc;

void CQuestManager::GetAllActiveQuestInfo(
    CCharStringVector& questNames,
    CCharStringVector& questInfo)
{
    g_managerThis = this;
    g_namesArgument = &questNames;
    g_infoArgument = &questInfo;
    ++g_callCount;
    questNames.marker = 0x11223344;
    questInfo.marker = 0x55667788;
}

void __fastcall CGameScriptInterface_GetAllActiveQuestInfo(
    void*,
    unsigned long,
    CCharStringVector& questNames,
    CCharStringVector& questInfo)
{
    DAT_013b89fc->GetAllActiveQuestInfo(questNames, questInfo);
}

int main()
{
    int failures = 0;
    CQuestManager manager;
    CCharStringVector names;
    CCharStringVector info;

    g_managerThis = 0;
    g_namesArgument = 0;
    g_infoArgument = 0;
    g_callCount = 0;
    names.marker = -1;
    info.marker = -2;
    DAT_013b89fc = &manager;

    CGameScriptInterface_GetAllActiveQuestInfo(
        (void*)0x12345678,
        0,
        names,
        info);

    if (g_callCount != 1 || g_managerThis != &manager)
    {
        std::printf("quest manager dispatch failed\n");
        ++failures;
    }
    if (g_namesArgument != &names || g_infoArgument != &info)
    {
        std::printf("reference forwarding order failed\n");
        ++failures;
    }
    if (names.marker != 0x11223344 || info.marker != 0x55667788)
    {
        std::printf("mutable output propagation failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf("FSE2_00891530_TEST FAIL count=%d\n", failures);
        return 1;
    }

    std::printf("FSE2_00891530_TEST PASS\n");
    return 0;
}

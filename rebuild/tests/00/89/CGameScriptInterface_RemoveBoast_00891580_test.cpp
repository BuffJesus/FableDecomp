#include <cstdio>

// Standalone behavior fixture for
// CGameScriptInterface::RemoveBoast @ 0x00891580.
struct CCharString
{
    const char* text;

    CCharString(const CCharString& other);
    explicit CCharString(const char* value) : text(value) {}
};

struct CQuestManager
{
    void RemoveBoast(long questId, CCharString boast);
};

static const CCharString* g_copySource;
static void* g_copyDestination;
static int g_copyCount;
static void* g_managerThis;
static long g_questId;
static const char* g_boastText;
static int g_removeCount;

CQuestManager* DAT_013b89fc;

CCharString::CCharString(const CCharString& other)
    : text(other.text)
{
    g_copySource = &other;
    g_copyDestination = this;
    ++g_copyCount;
}

void CQuestManager::RemoveBoast(long questId, CCharString boast)
{
    g_managerThis = this;
    g_questId = questId;
    g_boastText = boast.text;
    ++g_removeCount;
}

void __fastcall CGameScriptInterface_RemoveBoast(
    void*,
    unsigned long,
    long questId,
    const CCharString& boast)
{
    CQuestManager* questManager = DAT_013b89fc;
    questManager->RemoveBoast(questId, CCharString(boast));
}

static void ResetRecorders()
{
    g_copySource = 0;
    g_copyDestination = 0;
    g_copyCount = 0;
    g_managerThis = 0;
    g_questId = 0;
    g_boastText = 0;
    g_removeCount = 0;
}

int main()
{
    int failures = 0;
    CQuestManager manager;
    CCharString boast("BOAST_BANDIT_RAID");

    ResetRecorders();
    DAT_013b89fc = &manager;
    CGameScriptInterface_RemoveBoast(
        (void*)0x12345678,
        0,
        0x10203040,
        boast);

    if (g_copyCount != 1 ||
        g_copySource != &boast ||
        g_copyDestination == &boast)
    {
        std::printf("copy construction contract failed\n");
        ++failures;
    }
    if (g_removeCount != 1 ||
        g_managerThis != &manager ||
        g_questId != 0x10203040 ||
        g_boastText != boast.text)
    {
        std::printf("quest manager forwarding failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf("FSE2_00891580_TEST FAIL count=%d\n", failures);
        return 1;
    }

    std::printf("FSE2_00891580_TEST PASS\n");
    return 0;
}

#include <cstdio>

// Standalone behavior fixture for
// CGameScriptInterface::IsBoastTaken @ 0x00891550.
struct CCharString
{
    const char* text;

    CCharString(const CCharString& other);
    explicit CCharString(const char* value) : text(value) {}
};

struct CQuestManager
{
    bool IsBoastTaken(long questId, CCharString boast);
};

static const CCharString* g_copySource;
static void* g_copyDestination;
static int g_copyCount;
static void* g_managerThis;
static long g_questId;
static const char* g_boastText;
static int g_queryCount;
static bool g_queryResult;

CQuestManager* DAT_013b89fc;

CCharString::CCharString(const CCharString& other)
    : text(other.text)
{
    g_copySource = &other;
    g_copyDestination = this;
    ++g_copyCount;
}

bool CQuestManager::IsBoastTaken(long questId, CCharString boast)
{
    g_managerThis = this;
    g_questId = questId;
    g_boastText = boast.text;
    ++g_queryCount;
    return g_queryResult;
}

bool __fastcall CGameScriptInterface_IsBoastTaken(
    void*,
    unsigned long,
    long questId,
    const CCharString& boast)
{
    CQuestManager* questManager = DAT_013b89fc;
    return questManager->IsBoastTaken(questId, CCharString(boast));
}

static void ResetRecorders()
{
    g_copySource = 0;
    g_copyDestination = 0;
    g_copyCount = 0;
    g_managerThis = 0;
    g_questId = 0;
    g_boastText = 0;
    g_queryCount = 0;
}

static int CheckQuery(
    CQuestManager* manager,
    CCharString* boast,
    long questId,
    bool expected)
{
    ResetRecorders();
    DAT_013b89fc = manager;
    g_queryResult = expected;

    bool result = CGameScriptInterface_IsBoastTaken(
        (void*)0x12345678,
        0,
        questId,
        *boast);

    if (result != expected)
    {
        std::printf("return propagation failed\n");
        return 1;
    }
    if (g_copyCount != 1 ||
        g_copySource != boast ||
        g_copyDestination == boast)
    {
        std::printf("copy construction contract failed\n");
        return 1;
    }
    if (g_queryCount != 1 ||
        g_managerThis != manager ||
        g_questId != questId ||
        g_boastText != boast->text)
    {
        std::printf("quest manager forwarding failed\n");
        return 1;
    }
    return 0;
}

int main()
{
    int failures = 0;
    CQuestManager manager;
    CCharString boast("BOAST_HOBBE_CAVE");

    failures += CheckQuery(&manager, &boast, 73, false);
    failures += CheckQuery(&manager, &boast, 74, true);

    if (failures != 0)
    {
        std::printf("FSE2_00891550_TEST FAIL count=%d\n", failures);
        return 1;
    }

    std::printf("FSE2_00891550_TEST PASS\n");
    return 0;
}

#include <cstdio>

class CCharString
{
public:
    explicit CCharString(unsigned long identity)
        : m_Identity(identity)
    {
    }

    unsigned long m_Identity;
};

class CRegion
{
public:
    explicit CRegion(unsigned long identity)
        : m_Identity(identity)
    {
    }

    unsigned long m_Identity;
};

class CWorldMap;

class CWorld
{
public:
    virtual void Slot00() {}
    virtual void Slot04() {}
    virtual void Slot08() {}
    virtual void Slot0C() {}
    virtual void Slot10() {}
    virtual void Slot14() {}
    virtual void Slot18() {}
    virtual void Slot1C() {}
    virtual void Slot20() {}
    virtual void Slot24() {}
    virtual void Slot28() {}
    virtual void Slot2C() {}
    virtual void Slot30() {}
    virtual CWorldMap* GetWorldMap() = 0;
};

class CWorldMap
{
public:
    long GetRegionNumberFromName(
        const CCharString& regionName);
    CRegion& GetRegion(long regionNumber);
};

class CQuestManager
{
public:
    void AddQuestRegion(
        const CCharString& questName,
        const CRegion* region);
};

class CTestWorld : public CWorld
{
public:
    virtual CWorldMap* GetWorldMap();
};

class CGameScriptInterface
{
public:
    CGameScriptInterface()
        : m_World(0)
    {
    }

    virtual void AddQuestRegion(
        const CCharString& questName,
        const CCharString& regionName) const;

    CWorld* m_World;
};

CQuestManager* CGSIAddQuestRegion_QuestManager;
static CWorld* g_ObservedWorld;
static CWorldMap* g_WorldMap;
static CWorldMap* g_ObservedLookupWorldMap;
static const CCharString* g_ObservedRegionName;
static long g_RegionNumber;
static CWorldMap* g_ObservedGetRegionWorldMap;
static long g_ObservedRegionNumber;
static CRegion* g_Region;
static CQuestManager* g_ObservedQuestManager;
static const CCharString* g_ObservedQuestName;
static const CRegion* g_ObservedRegion;
static unsigned long g_GetWorldMapCallCount;
static unsigned long g_LookupCallCount;
static unsigned long g_GetRegionCallCount;
static unsigned long g_AddQuestRegionCallCount;

CWorldMap* CTestWorld::GetWorldMap()
{
    g_ObservedWorld = this;
    ++g_GetWorldMapCallCount;
    return g_WorldMap;
}

long CWorldMap::GetRegionNumberFromName(
    const CCharString& regionName)
{
    g_ObservedLookupWorldMap = this;
    g_ObservedRegionName = &regionName;
    ++g_LookupCallCount;
    return g_RegionNumber;
}

CRegion& CWorldMap::GetRegion(long regionNumber)
{
    g_ObservedGetRegionWorldMap = this;
    g_ObservedRegionNumber = regionNumber;
    ++g_GetRegionCallCount;
    return *g_Region;
}

void CQuestManager::AddQuestRegion(
    const CCharString& questName,
    const CRegion* region)
{
    g_ObservedQuestManager = this;
    g_ObservedQuestName = &questName;
    g_ObservedRegion = region;
    ++g_AddQuestRegionCallCount;
}

static void ResetObservations()
{
    g_ObservedWorld = 0;
    g_ObservedLookupWorldMap = 0;
    g_ObservedRegionName = 0;
    g_ObservedGetRegionWorldMap = 0;
    g_ObservedRegionNumber = 0;
    g_ObservedQuestManager = 0;
    g_ObservedQuestName = 0;
    g_ObservedRegion = 0;
    g_GetWorldMapCallCount = 0;
    g_LookupCallCount = 0;
    g_GetRegionCallCount = 0;
    g_AddQuestRegionCallCount = 0;
}

int main()
{
    CTestWorld world;
    CWorldMap worldMap;
    CQuestManager questManager;
    CGameScriptInterface interfaceObject;
    CRegion region(0x10203040);
    CCharString questName(0x11223344);
    CCharString missingRegionName(0x55667788);
    CCharString presentRegionName(0x99AABBCC);
    interfaceObject.m_World = &world;
    g_WorldMap = &worldMap;
    g_Region = &region;
    CGSIAddQuestRegion_QuestManager = &questManager;

    ResetObservations();
    g_RegionNumber = -1;
    interfaceObject.AddQuestRegion(
        questName,
        missingRegionName);
    if (g_ObservedWorld != &world ||
        g_ObservedLookupWorldMap != &worldMap ||
        g_ObservedRegionName != &missingRegionName ||
        g_GetWorldMapCallCount != 1 ||
        g_LookupCallCount != 1 ||
        g_GetRegionCallCount != 0 ||
        g_AddQuestRegionCallCount != 0)
        return 1;

    ResetObservations();
    g_RegionNumber = 0;
    interfaceObject.AddQuestRegion(
        questName,
        missingRegionName);
    if (g_ObservedWorld != &world ||
        g_ObservedLookupWorldMap != &worldMap ||
        g_ObservedRegionName != &missingRegionName ||
        g_GetWorldMapCallCount != 1 ||
        g_LookupCallCount != 1 ||
        g_GetRegionCallCount != 0 ||
        g_AddQuestRegionCallCount != 0)
        return 2;

    ResetObservations();
    g_RegionNumber = 0x7FFFFFFF;
    interfaceObject.AddQuestRegion(
        questName,
        presentRegionName);
    if (g_ObservedWorld != &world ||
        g_ObservedLookupWorldMap != &worldMap ||
        g_ObservedRegionName != &presentRegionName ||
        g_ObservedGetRegionWorldMap != &worldMap ||
        g_ObservedRegionNumber != 0x7FFFFFFF ||
        g_ObservedQuestManager != &questManager ||
        g_ObservedQuestName != &questName ||
        g_ObservedRegion != &region ||
        g_GetWorldMapCallCount != 2 ||
        g_LookupCallCount != 1 ||
        g_GetRegionCallCount != 1 ||
        g_AddQuestRegionCallCount != 1)
        return 3;

    std::printf("FSE2_00891820_TEST PASS\n");
    return 0;
}

#include <cstdio>

class CCharString
{
public:
    unsigned long m_Marker;
};

class CWorldMap
{
public:
    long GetRegionNumberFromName(const CCharString& regionName);
};

class CGSIHenchmenRegion_InterfaceSource
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
    virtual void* GetInterface();
};

class CGSIHenchmenRegion_State
{
public:
    virtual void Slot00();
    virtual void Release();

    long m_RefCount;
    unsigned char m_Pad08[0x81];
    bool m_IsAllowed;
};

struct CGSIHenchmenRegion_Category
{
    unsigned char m_Pad00[0x24];
    CGSIHenchmenRegion_State* m_pState;
};

namespace NInventory
{
class CTCInventoryBase
{
public:
    CGSIHenchmenRegion_Category* GetCategoryWithIndex(long index);
};
}

struct CGSIHenchmenRegion_InterfaceOverlay
{
    unsigned char m_Pad00[0x04];
    CGSIHenchmenRegion_InterfaceSource* m_pInterfaceSource;
};

class CGameScriptInterface
{
public:
    virtual bool IsHeroAllowedHenchmenInRegion(
        const CCharString& regionName) const;
};

static unsigned long g_InterfaceCalls;
static unsigned long g_RegionCalls;
static unsigned long g_CategoryCalls;
static unsigned long g_ReleaseCalls;
static bool g_ReturnWorldMapNext;
static CWorldMap* g_pExpectedWorldMap;
static NInventory::CTCInventoryBase* g_pExpectedInventory;
static const CCharString* g_pExpectedRegionName;
static long g_RegionNumber;
static CGSIHenchmenRegion_Category* g_pCategory;

void* CGSIHenchmenRegion_InterfaceSource::GetInterface()
{
    ++g_InterfaceCalls;
    if (g_ReturnWorldMapNext)
    {
        g_ReturnWorldMapNext = false;
        return g_pExpectedWorldMap;
    }
    return g_pExpectedInventory;
}

long CWorldMap::GetRegionNumberFromName(const CCharString& regionName)
{
    if (this != g_pExpectedWorldMap)
    {
        std::printf("world-map this mismatch\n");
        return 0;
    }
    if (&regionName != g_pExpectedRegionName)
    {
        std::printf("region-name mismatch\n");
        return 0;
    }
    ++g_RegionCalls;
    return g_RegionNumber;
}

CGSIHenchmenRegion_Category*
NInventory::CTCInventoryBase::GetCategoryWithIndex(long index)
{
    if (this != g_pExpectedInventory)
    {
        std::printf("inventory this mismatch\n");
        return 0;
    }
    if (index != g_RegionNumber)
    {
        std::printf("region-index mismatch\n");
        return 0;
    }
    ++g_CategoryCalls;
    return g_pCategory;
}

void CGSIHenchmenRegion_State::Slot00()
{
}

void CGSIHenchmenRegion_State::Release()
{
    ++g_ReleaseCalls;
}

static int Fail(const char* pMessage)
{
    std::printf("FSE2_00891ef0_TEST FAIL: %s\n", pMessage);
    return 1;
}

int main()
{
    CGSIHenchmenRegion_InterfaceSource interfaceSource;
    CGSIHenchmenRegion_InterfaceOverlay interfaceStorage = {};
    CGSIHenchmenRegion_Category category = {};
    CGSIHenchmenRegion_State state;
    CCharString regionName;
    CGameScriptInterface& interfaceObject =
        reinterpret_cast<CGameScriptInterface&>(interfaceStorage);

    g_pExpectedWorldMap = reinterpret_cast<CWorldMap*>(0x12345678);
    g_pExpectedInventory =
        reinterpret_cast<NInventory::CTCInventoryBase*>(0x23456789);
    g_pExpectedRegionName = &regionName;
    g_pCategory = &category;
    interfaceStorage.m_pInterfaceSource = &interfaceSource;
    category.m_pState = &state;
    regionName.m_Marker = 0xA5A5A5A5;

    g_RegionNumber = -17;
    g_ReturnWorldMapNext = true;
    bool allowed =
        interfaceObject.CGameScriptInterface::
            IsHeroAllowedHenchmenInRegion(regionName);
    if (!allowed || g_CategoryCalls != 0)
        return Fail("negative region default");

    g_RegionNumber = 0;
    g_ReturnWorldMapNext = true;
    allowed =
        interfaceObject.CGameScriptInterface::
            IsHeroAllowedHenchmenInRegion(regionName);
    if (!allowed || g_CategoryCalls != 0)
        return Fail("zero region default");

    g_RegionNumber = 23;
    category.m_pState = 0;
    g_ReturnWorldMapNext = true;
    allowed =
        interfaceObject.CGameScriptInterface::
            IsHeroAllowedHenchmenInRegion(regionName);
    if (!allowed || g_CategoryCalls != 1)
        return Fail("missing state default");

    category.m_pState = &state;
    state.m_RefCount = 9;
    state.m_IsAllowed = false;
    g_ReturnWorldMapNext = true;
    allowed =
        interfaceObject.CGameScriptInterface::
            IsHeroAllowedHenchmenInRegion(regionName);
    if (allowed || state.m_RefCount != 9 || g_ReleaseCalls != 0)
        return Fail("retained false state");

    state.m_RefCount = 0;
    state.m_IsAllowed = true;
    g_ReturnWorldMapNext = true;
    allowed =
        interfaceObject.CGameScriptInterface::
            IsHeroAllowedHenchmenInRegion(regionName);
    if (!allowed || state.m_RefCount != 0 || g_ReleaseCalls != 1)
        return Fail("released true state");

    if (g_InterfaceCalls != 8 || g_RegionCalls != 5 ||
        g_CategoryCalls != 3)
        return Fail("call counts");

    std::printf("FSE2_00891ef0_TEST PASS\n");
    return 0;
}

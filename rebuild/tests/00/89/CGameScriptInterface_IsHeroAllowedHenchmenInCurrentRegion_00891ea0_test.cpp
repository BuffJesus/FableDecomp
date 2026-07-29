#include <cstdio>

class CIDrawEnvironment
{
};

class CWorld
{
public:
    CIDrawEnvironment* DrawGetEnvironment();
};

class CGSIHenchmenCurrentRegion_InterfaceSource
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

class CGSIHenchmenCurrentRegion_State
{
public:
    virtual void Slot00();
    virtual void Release();

    volatile long m_RefCount;
    unsigned char m_Pad08[0x81];
    bool m_IsAllowed;
};

struct CGSIHenchmenCurrentRegion_Category
{
    unsigned char m_Pad00[0x24];
    CGSIHenchmenCurrentRegion_State* m_pState;
};

namespace NInventory
{
class CTCInventoryBase
{
public:
    CGSIHenchmenCurrentRegion_Category* GetCategoryWithIndex(long index);
};
}

struct CGSIHenchmenCurrentRegion_InterfaceOverlay
{
    unsigned char m_Pad00[0x04];
    CGSIHenchmenCurrentRegion_InterfaceSource* m_pInterfaceSource;
};

class CGameScriptInterface
{
public:
    virtual bool IsHeroAllowedHenchmenInCurrentRegion() const;
};

static unsigned long g_InterfaceCalls;
static unsigned long g_DrawCalls;
static unsigned long g_CategoryCalls;
static unsigned long g_ReleaseCalls;
static CWorld* g_pExpectedWorld;
static NInventory::CTCInventoryBase* g_pExpectedInventory;
static CIDrawEnvironment* g_pEnvironment;
static CGSIHenchmenCurrentRegion_Category* g_pCategory;

void* CGSIHenchmenCurrentRegion_InterfaceSource::GetInterface()
{
    ++g_InterfaceCalls;
    if ((g_InterfaceCalls & 1) != 0)
        return g_pExpectedWorld;
    return g_pExpectedInventory;
}

CIDrawEnvironment* CWorld::DrawGetEnvironment()
{
    if (this != g_pExpectedWorld)
    {
        std::printf("world this mismatch\n");
        return 0;
    }
    ++g_DrawCalls;
    return g_pEnvironment;
}

CGSIHenchmenCurrentRegion_Category*
NInventory::CTCInventoryBase::GetCategoryWithIndex(long index)
{
    if (this != g_pExpectedInventory)
    {
        std::printf("inventory this mismatch\n");
        return 0;
    }
    if (index != reinterpret_cast<long>(g_pEnvironment))
    {
        std::printf("category index mismatch\n");
        return 0;
    }
    ++g_CategoryCalls;
    return g_pCategory;
}

void CGSIHenchmenCurrentRegion_State::Slot00()
{
}

void CGSIHenchmenCurrentRegion_State::Release()
{
    ++g_ReleaseCalls;
}

static int Fail(const char* pMessage)
{
    std::printf("FSE2_00891ea0_TEST FAIL: %s\n", pMessage);
    return 1;
}

int main()
{
    CGSIHenchmenCurrentRegion_InterfaceSource interfaceSource;
    CGSIHenchmenCurrentRegion_InterfaceOverlay interfaceStorage = {};
    CGSIHenchmenCurrentRegion_Category category = {};
    CGSIHenchmenCurrentRegion_State state;
    CGameScriptInterface& interfaceObject =
        reinterpret_cast<CGameScriptInterface&>(interfaceStorage);

    g_pExpectedWorld = reinterpret_cast<CWorld*>(0x12345678);
    g_pExpectedInventory =
        reinterpret_cast<NInventory::CTCInventoryBase*>(0x23456789);
    g_pEnvironment = reinterpret_cast<CIDrawEnvironment*>(0x3456789A);
    g_pCategory = &category;
    interfaceStorage.m_pInterfaceSource = &interfaceSource;
    category.m_pState = &state;

    state.m_RefCount = 7;
    state.m_IsAllowed = false;
    bool allowed =
        interfaceObject.CGameScriptInterface::
            IsHeroAllowedHenchmenInCurrentRegion();
    if (allowed)
        return Fail("false result");
    if (state.m_RefCount != 7 || g_ReleaseCalls != 0)
        return Fail("retained reference");

    state.m_RefCount = 0;
    state.m_IsAllowed = true;
    allowed =
        interfaceObject.CGameScriptInterface::
            IsHeroAllowedHenchmenInCurrentRegion();
    if (!allowed)
        return Fail("true result");
    if (state.m_RefCount != 0 || g_ReleaseCalls != 1)
        return Fail("temporary reference release");

    category.m_pState = 0;
    allowed =
        interfaceObject.CGameScriptInterface::
            IsHeroAllowedHenchmenInCurrentRegion();
    if (!allowed)
        return Fail("missing state default");
    if (g_InterfaceCalls != 6 || g_DrawCalls != 3 || g_CategoryCalls != 3)
        return Fail("lookup call counts");

    std::printf("FSE2_00891ea0_TEST PASS\n");
    return 0;
}

#include <cstdio>

class CGSIHeroWill_Player;
class CGSIHeroWill_Node;
class CGSIHeroWill_Manager
{
public:
    CGSIHeroWill_Player* GetPlayer();
};
class CGSIHeroWill_Player
{
public:
    void* GetHero();
};
class CGSIHeroWill_Container
{
public:
    CGSIHeroWill_Node* LowerBound(const long*);
    CGSIHeroWill_Node* m_pFirst;
    CGSIHeroWill_Node* m_pEnd;
};
class CGSIHeroWill_Component
{
public:
    long GetCategoryLevel(long);
    long m_Result;
};
class CGSIHeroWill_Node
{
public:
    long m_Key;
    CGSIHeroWill_Component* m_pComponent;
};
class CGameScriptInterface
{
public:
    virtual long GetHeroWillLevel() const;
};
struct InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIHeroWill_Manager* m_pManager;
};
struct EntityStorage
{
    unsigned char m_Pad00[0x2C];
    unsigned long m_Flags2C;
    unsigned char m_Pad30[0x14];
    CGSIHeroWill_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIHeroWill_Player* g_pPlayer;
static EntityStorage* g_pHero;
static CGSIHeroWill_Node* g_pLowerBoundResult;
static void* g_pFallback;
static long g_FallbackResult;
static CGSIHeroWill_Component* g_pObservedComponent;
static CGSIHeroWill_Container* g_pObservedContainer;
static long g_ObservedKey;
static long g_ObservedCategory;

CGSIHeroWill_Player* CGSIHeroWill_Manager::GetPlayer()
{
    return g_pPlayer;
}
void* CGSIHeroWill_Player::GetHero()
{
    return g_pHero;
}
CGSIHeroWill_Node*
CGSIHeroWill_Container::LowerBound(const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    return g_pLowerBoundResult;
}
long CGSIHeroWill_Component::GetCategoryLevel(long category)
{
    g_pObservedComponent = this;
    g_ObservedCategory = category;
    return this == g_pFallback ? g_FallbackResult : m_Result;
}
static void Reset()
{
    g_pObservedComponent = 0;
    g_pObservedContainer = 0;
    g_ObservedKey = -1;
    g_ObservedCategory = -1;
}
static int Fail(const char* message)
{
    std::printf("FSE2_00899470_TEST FAIL: %s\n", message);
    return 1;
}

int main()
{
    InterfaceStorage iface = {};
    CGSIHeroWill_Manager manager;
    CGSIHeroWill_Player player;
    EntityStorage hero = {};
    CGSIHeroWill_Component found = {};
    CGSIHeroWill_Component end = {};
    CGSIHeroWill_Node foundNode = {};
    CGSIHeroWill_Node endNode = {};
    iface.m_pManager = &manager;
    CGameScriptInterface* pInterface =
        reinterpret_cast<CGameScriptInterface*>(&iface);
    g_pFallback = &iface;
    g_FallbackResult = 14;
    g_pPlayer = &player;
    foundNode.m_pComponent = &found;
    endNode.m_pComponent = &end;
    hero.m_Container.m_pEnd = &endNode;

    g_pHero = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroWillLevel() != -1)
        return Fail("null hero");
    g_pHero = &hero;
    hero.m_Flags91 = 1;
    if (pInterface->CGameScriptInterface::
            GetHeroWillLevel() != -1)
        return Fail("dead hero");

    Reset();
    hero.m_Flags91 = 0;
    hero.m_Flags2C = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroWillLevel() != 14 ||
        g_pObservedComponent !=
            reinterpret_cast<CGSIHeroWill_Component*>(&iface) ||
        g_ObservedCategory != 2)
        return Fail("fallback category two");

    Reset();
    hero.m_Flags2C = 0x100;
    foundNode.m_Key = 0x68;
    found.m_Result = 28;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroWillLevel() != 28 ||
        g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 0x68 ||
        g_pObservedComponent != &found ||
        g_ObservedCategory != 2)
        return Fail("found component");

    Reset();
    foundNode.m_Key = 0x69;
    end.m_Result = 42;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroWillLevel() != 42 ||
        g_pObservedComponent != &end)
        return Fail("greater-key sentinel");
    Reset();
    g_pLowerBoundResult = &endNode;
    if (pInterface->CGameScriptInterface::
            GetHeroWillLevel() != 42)
        return Fail("direct sentinel");

    std::printf("FSE2_00899470_TEST PASS\n");
    return 0;
}

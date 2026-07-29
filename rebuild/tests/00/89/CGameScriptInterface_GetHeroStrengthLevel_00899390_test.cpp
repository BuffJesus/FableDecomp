#include <cstdio>

class CGSIHeroStrength_Player;
class CGSIHeroStrength_Node;
class CGSIHeroStrength_Manager
{
public:
    CGSIHeroStrength_Player* GetPlayer();
};
class CGSIHeroStrength_Player
{
public:
    void* GetHero();
};
class CGSIHeroStrength_Container
{
public:
    CGSIHeroStrength_Node* LowerBound(const long*);
    CGSIHeroStrength_Node* m_pFirst;
    CGSIHeroStrength_Node* m_pEnd;
};
class CGSIHeroStrength_Component
{
public:
    long GetCategoryLevel(long);
    long m_Result;
};
class CGSIHeroStrength_Node
{
public:
    long m_Key;
    CGSIHeroStrength_Component* m_pComponent;
};
class CGameScriptInterface
{
public:
    virtual long GetHeroStrengthLevel() const;
};
struct InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIHeroStrength_Manager* m_pManager;
};
struct EntityStorage
{
    unsigned char m_Pad00[0x2C];
    unsigned long m_Flags2C;
    unsigned char m_Pad30[0x14];
    CGSIHeroStrength_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIHeroStrength_Player* g_pPlayer;
static EntityStorage* g_pHero;
static CGSIHeroStrength_Node* g_pLowerBoundResult;
static void* g_pFallback;
static long g_FallbackResult;
static CGSIHeroStrength_Component* g_pObservedComponent;
static CGSIHeroStrength_Container* g_pObservedContainer;
static long g_ObservedKey;
static long g_ObservedCategory;

CGSIHeroStrength_Player*
CGSIHeroStrength_Manager::GetPlayer()
{
    return g_pPlayer;
}
void* CGSIHeroStrength_Player::GetHero()
{
    return g_pHero;
}
CGSIHeroStrength_Node*
CGSIHeroStrength_Container::LowerBound(const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    return g_pLowerBoundResult;
}
long CGSIHeroStrength_Component::GetCategoryLevel(long category)
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
    std::printf("FSE2_00899390_TEST FAIL: %s\n", message);
    return 1;
}

int main()
{
    InterfaceStorage iface = {};
    CGSIHeroStrength_Manager manager;
    CGSIHeroStrength_Player player;
    EntityStorage hero = {};
    CGSIHeroStrength_Component found = {};
    CGSIHeroStrength_Component end = {};
    CGSIHeroStrength_Node foundNode = {};
    CGSIHeroStrength_Node endNode = {};
    iface.m_pManager = &manager;
    CGameScriptInterface* pInterface =
        reinterpret_cast<CGameScriptInterface*>(&iface);
    g_pFallback = &iface;
    g_FallbackResult = 12;
    g_pPlayer = &player;
    foundNode.m_pComponent = &found;
    endNode.m_pComponent = &end;
    hero.m_Container.m_pEnd = &endNode;

    g_pHero = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroStrengthLevel() != -1)
        return Fail("null hero");
    g_pHero = &hero;
    hero.m_Flags91 = 1;
    if (pInterface->CGameScriptInterface::
            GetHeroStrengthLevel() != -1)
        return Fail("dead hero");

    Reset();
    hero.m_Flags91 = 0;
    hero.m_Flags2C = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroStrengthLevel() != 12 ||
        g_pObservedComponent !=
            reinterpret_cast<CGSIHeroStrength_Component*>(&iface) ||
        g_ObservedCategory != 0)
        return Fail("fallback category zero");

    Reset();
    hero.m_Flags2C = 0x100;
    foundNode.m_Key = 0x68;
    found.m_Result = 24;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroStrengthLevel() != 24 ||
        g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 0x68 ||
        g_pObservedComponent != &found ||
        g_ObservedCategory != 0)
        return Fail("found component");

    Reset();
    foundNode.m_Key = 0x69;
    end.m_Result = 36;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroStrengthLevel() != 36 ||
        g_pObservedComponent != &end)
        return Fail("greater-key sentinel");
    Reset();
    g_pLowerBoundResult = &endNode;
    if (pInterface->CGameScriptInterface::
            GetHeroStrengthLevel() != 36)
        return Fail("direct sentinel");

    std::printf("FSE2_00899390_TEST PASS\n");
    return 0;
}

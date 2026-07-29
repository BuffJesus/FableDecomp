#include <cstdio>

class CGSIHeroSkill_Player;
class CGSIHeroSkill_Node;

class CGSIHeroSkill_Manager
{
public:
    CGSIHeroSkill_Player* GetPlayer();
};

class CGSIHeroSkill_Player
{
public:
    void* GetHero();
};

class CGSIHeroSkill_Container
{
public:
    CGSIHeroSkill_Node* LowerBound(const long* pKey);

    CGSIHeroSkill_Node* m_pFirst;
    CGSIHeroSkill_Node* m_pEnd;
};

class CGSIHeroSkill_Component
{
public:
    long GetCategoryLevel(long category);

    long m_Result;
};

class CGSIHeroSkill_Node
{
public:
    long m_Key;
    CGSIHeroSkill_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual long GetHeroSkillLevel() const;
};

struct InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIHeroSkill_Manager* m_pManager;
};

struct EntityStorage
{
    unsigned char m_Pad00[0x2C];
    unsigned long m_Flags2C;
    unsigned char m_Pad30[0x14];
    CGSIHeroSkill_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIHeroSkill_Player* g_pPlayer;
static EntityStorage* g_pHero;
static CGSIHeroSkill_Node* g_pLowerBoundResult;
static void* g_pFallbackComponent;
static long g_FallbackResult;
static CGSIHeroSkill_Container* g_pObservedContainer;
static CGSIHeroSkill_Component* g_pObservedComponent;
static long g_ObservedKey;
static long g_ObservedCategory;
static unsigned long g_LowerBoundCalls;
static unsigned long g_QueryCalls;

CGSIHeroSkill_Player* CGSIHeroSkill_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIHeroSkill_Player::GetHero()
{
    return g_pHero;
}

CGSIHeroSkill_Node*
CGSIHeroSkill_Container::LowerBound(const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

long CGSIHeroSkill_Component::GetCategoryLevel(
    long category)
{
    g_pObservedComponent = this;
    g_ObservedCategory = category;
    ++g_QueryCalls;
    if (this == g_pFallbackComponent)
        return g_FallbackResult;
    return m_Result;
}

static void ResetObservations()
{
    g_pObservedContainer = 0;
    g_pObservedComponent = 0;
    g_ObservedKey = 0;
    g_ObservedCategory = -1;
    g_LowerBoundCalls = 0;
    g_QueryCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_00899400_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    InterfaceStorage interfaceStorage = {};
    CGSIHeroSkill_Manager manager;
    CGSIHeroSkill_Player player;
    EntityStorage hero = {};
    CGSIHeroSkill_Component foundComponent = {};
    CGSIHeroSkill_Component endComponent = {};
    CGSIHeroSkill_Node foundNode = {};
    CGSIHeroSkill_Node endNode = {};

    interfaceStorage.m_pManager = &manager;
    CGameScriptInterface* const pInterface =
        reinterpret_cast<CGameScriptInterface*>(
            &interfaceStorage);
    g_pFallbackComponent = &interfaceStorage;
    g_FallbackResult = 17;
    g_pPlayer = &player;
    foundNode.m_pComponent = &foundComponent;
    endNode.m_pComponent = &endComponent;
    hero.m_Container.m_pEnd = &endNode;

    g_pHero = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroSkillLevel() != -1)
        return Fail("null hero result");

    g_pHero = &hero;
    hero.m_Flags91 = 1;
    if (pInterface->CGameScriptInterface::
            GetHeroSkillLevel() != -1)
        return Fail("dead hero result");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags2C = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroSkillLevel() != 17 ||
        g_pObservedComponent !=
            reinterpret_cast<CGSIHeroSkill_Component*>(
                &interfaceStorage) ||
        g_ObservedCategory != 1 ||
        g_LowerBoundCalls != 0 || g_QueryCalls != 1)
        return Fail("interface fallback category");

    ResetObservations();
    hero.m_Flags2C = 0x100;
    foundNode.m_Key = 0x68;
    foundComponent.m_Result = 23;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroSkillLevel() != 23 ||
        g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 0x68 ||
        g_pObservedComponent != &foundComponent ||
        g_ObservedCategory != 1)
        return Fail("found skill component");

    ResetObservations();
    foundNode.m_Key = 0x69;
    endComponent.m_Result = 31;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroSkillLevel() != 31 ||
        g_pObservedComponent != &endComponent)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    if (pInterface->CGameScriptInterface::
            GetHeroSkillLevel() != 31)
        return Fail("direct sentinel");

    std::printf("FSE2_00899400_TEST PASS\n");
    return 0;
}

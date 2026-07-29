#include <cstdio>

class CGSIHeroAge_Player;
class CGSIHeroAge_Node;

class CGSIHeroAge_Manager
{
public:
    CGSIHeroAge_Player* GetPlayer();
};

class CGSIHeroAge_Player
{
public:
    void* GetHero();
};

class CGSIHeroAge_Container
{
public:
    CGSIHeroAge_Node* LowerBound(
        const long* pKey);

    CGSIHeroAge_Node* m_pFirst;
    CGSIHeroAge_Node* m_pEnd;
};

class CGSIHeroAge_Component
{
public:
    void SetAge(float age);
};

class CGSIHeroAge_Node
{
public:
    long m_Key;
    CGSIHeroAge_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual void SetHeroAge(float age) const;
};

struct CGSIHeroAge_InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIHeroAge_Manager* m_pManager;
};

struct CGSIHeroAge_HeroStorage
{
    unsigned char m_Pad00[0x20];
    unsigned long m_Flags20;
    unsigned char m_Pad24[0x20];
    CGSIHeroAge_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

union CGSIHeroAge_FloatBits
{
    float m_Float;
    unsigned long m_Bits;
};

static CGSIHeroAge_Player* g_pPlayer;
static CGSIHeroAge_HeroStorage* g_pHero;
static CGSIHeroAge_Node* g_pLowerBoundResult;
static CGSIHeroAge_Container* g_pObservedContainer;
static CGSIHeroAge_Component* g_pObservedComponent;
static long g_ObservedKey;
static CGSIHeroAge_FloatBits g_ObservedAge;
static unsigned long g_LowerBoundCalls;
static unsigned long g_SetAgeCalls;

CGSIHeroAge_Player* CGSIHeroAge_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIHeroAge_Player::GetHero()
{
    return g_pHero;
}

CGSIHeroAge_Node*
CGSIHeroAge_Container::LowerBound(const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

void CGSIHeroAge_Component::SetAge(float age)
{
    CGSIHeroAge_FloatBits bits;
    bits.m_Float = age;
    g_pObservedComponent = this;
    g_ObservedAge.m_Bits = bits.m_Bits;
    ++g_SetAgeCalls;
}

static void ResetObservations()
{
    g_pObservedContainer = 0;
    g_pObservedComponent = 0;
    g_ObservedKey = -1;
    g_ObservedAge.m_Bits = 0;
    g_LowerBoundCalls = 0;
    g_SetAgeCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_008994e0_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    CGSIHeroAge_InterfaceStorage interfaceStorage = {};
    CGSIHeroAge_Manager manager;
    CGSIHeroAge_Player player;
    CGSIHeroAge_HeroStorage hero = {};
    CGSIHeroAge_Component fallbackComponent;
    CGSIHeroAge_Component foundComponent;
    CGSIHeroAge_Component endComponent;
    CGSIHeroAge_Node foundNode = {};
    CGSIHeroAge_Node endNode = {};

    interfaceStorage.m_pManager = &manager;
    CGameScriptInterface* const pInterface =
        reinterpret_cast<CGameScriptInterface*>(
            &interfaceStorage);
    g_pPlayer = &player;
    foundNode.m_pComponent = &foundComponent;
    endNode.m_pComponent = &endComponent;
    hero.m_Container.m_pEnd = &endNode;

    ResetObservations();
    g_pHero = 0;
    pInterface->CGameScriptInterface::SetHeroAge(18.0f);
    if (g_SetAgeCalls != 0)
        return Fail("null hero guard");

    g_pHero = &hero;
    ResetObservations();
    hero.m_Flags91 = 1;
    pInterface->CGameScriptInterface::SetHeroAge(18.0f);
    if (g_SetAgeCalls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0;
    CGSIHeroAge_FloatBits fallbackAge;
    fallbackAge.m_Bits =
        reinterpret_cast<unsigned long>(
            &fallbackComponent);
    pInterface->CGameScriptInterface::SetHeroAge(
        fallbackAge.m_Float);
    if (g_LowerBoundCalls != 0 ||
        g_SetAgeCalls != 1 ||
        g_pObservedComponent != &fallbackComponent ||
        g_ObservedAge.m_Bits != fallbackAge.m_Bits)
        return Fail("fallback bit-preserving dispatch");

    ResetObservations();
    hero.m_Flags20 = 0x10;
    foundNode.m_Key = 4;
    g_pLowerBoundResult = &foundNode;
    CGSIHeroAge_FloatBits requestedAge;
    requestedAge.m_Float = 33.5f;
    pInterface->CGameScriptInterface::SetHeroAge(
        requestedAge.m_Float);
    if (g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 4 ||
        g_pObservedComponent != &foundComponent ||
        g_ObservedAge.m_Bits != requestedAge.m_Bits ||
        g_LowerBoundCalls != 1 ||
        g_SetAgeCalls != 1)
        return Fail("found HeroStats component");

    ResetObservations();
    foundNode.m_Key = 5;
    g_pLowerBoundResult = &foundNode;
    pInterface->CGameScriptInterface::SetHeroAge(44.0f);
    if (g_pObservedComponent != &endComponent)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    pInterface->CGameScriptInterface::SetHeroAge(55.0f);
    if (g_pObservedComponent != &endComponent)
        return Fail("direct sentinel");

    std::printf("FSE2_008994e0_TEST PASS\n");
    return 0;
}

#include <cstdio>

class CGSIWillEnergy_Player;
class CGSIWillEnergy_Node;

class CGSIWillEnergy_Manager
{
public:
    CGSIWillEnergy_Player* GetPlayer();
};

class CGSIWillEnergy_Player
{
public:
    void* GetHero();
};

class CGSIWillEnergy_Container
{
public:
    CGSIWillEnergy_Node* LowerBound(
        const long* pKey);

    CGSIWillEnergy_Node* m_pFirst;
    CGSIWillEnergy_Node* m_pEnd;
};

class CGSIWillEnergy_Component
{
public:
    void SetWillEnergyLevel(float level);
};

class CGSIWillEnergy_Node
{
public:
    long m_Key;
    CGSIWillEnergy_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual void SetHeroWillEnergyLevel(
        float level) const;
};

struct CGSIWillEnergy_InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIWillEnergy_Manager* m_pManager;
};

struct CGSIWillEnergy_HeroStorage
{
    unsigned char m_Pad00[0x20];
    unsigned long m_Flags20;
    unsigned char m_Pad24[0x20];
    CGSIWillEnergy_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

union CGSIWillEnergy_FloatBits
{
    float m_Float;
    unsigned long m_Bits;
};

static CGSIWillEnergy_Player* g_pPlayer;
static CGSIWillEnergy_HeroStorage* g_pHero;
static CGSIWillEnergy_Node* g_pLowerBoundResult;
static CGSIWillEnergy_Container* g_pObservedContainer;
static CGSIWillEnergy_Component* g_pObservedComponent;
static long g_ObservedKey;
static CGSIWillEnergy_FloatBits g_ObservedLevel;
static unsigned long g_LowerBoundCalls;
static unsigned long g_SetCalls;

CGSIWillEnergy_Player* CGSIWillEnergy_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIWillEnergy_Player::GetHero()
{
    return g_pHero;
}

CGSIWillEnergy_Node*
CGSIWillEnergy_Container::LowerBound(const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

void CGSIWillEnergy_Component::SetWillEnergyLevel(
    float level)
{
    CGSIWillEnergy_FloatBits bits;
    bits.m_Float = level;
    g_pObservedComponent = this;
    g_ObservedLevel.m_Bits = bits.m_Bits;
    ++g_SetCalls;
}

static void ResetObservations()
{
    g_pObservedContainer = 0;
    g_pObservedComponent = 0;
    g_ObservedKey = -1;
    g_ObservedLevel.m_Bits = 0;
    g_LowerBoundCalls = 0;
    g_SetCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_00899a10_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    CGSIWillEnergy_InterfaceStorage interfaceStorage = {};
    CGSIWillEnergy_Manager manager;
    CGSIWillEnergy_Player player;
    CGSIWillEnergy_HeroStorage hero = {};
    CGSIWillEnergy_Component fallbackComponent;
    CGSIWillEnergy_Component foundComponent;
    CGSIWillEnergy_Component endComponent;
    CGSIWillEnergy_Node foundNode = {};
    CGSIWillEnergy_Node endNode = {};

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
    pInterface->CGameScriptInterface::
        SetHeroWillEnergyLevel(10.0f);
    if (g_SetCalls != 0)
        return Fail("null hero guard");

    g_pHero = &hero;
    ResetObservations();
    hero.m_Flags91 = 1;
    pInterface->CGameScriptInterface::
        SetHeroWillEnergyLevel(10.0f);
    if (g_SetCalls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0;
    CGSIWillEnergy_FloatBits fallbackLevel;
    fallbackLevel.m_Bits =
        reinterpret_cast<unsigned long>(
            &fallbackComponent);
    pInterface->CGameScriptInterface::
        SetHeroWillEnergyLevel(
            fallbackLevel.m_Float);
    if (g_LowerBoundCalls != 0 ||
        g_SetCalls != 1 ||
        g_pObservedComponent != &fallbackComponent ||
        g_ObservedLevel.m_Bits != fallbackLevel.m_Bits)
        return Fail("fallback bit-preserving dispatch");

    ResetObservations();
    hero.m_Flags20 = 0x10;
    foundNode.m_Key = 4;
    g_pLowerBoundResult = &foundNode;
    CGSIWillEnergy_FloatBits requestedLevel;
    requestedLevel.m_Float = 62.5f;
    pInterface->CGameScriptInterface::
        SetHeroWillEnergyLevel(
            requestedLevel.m_Float);
    if (g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 4 ||
        g_pObservedComponent != &foundComponent ||
        g_ObservedLevel.m_Bits != requestedLevel.m_Bits ||
        g_LowerBoundCalls != 1 ||
        g_SetCalls != 1)
        return Fail("found HeroStats component");

    ResetObservations();
    foundNode.m_Key = 5;
    g_pLowerBoundResult = &foundNode;
    pInterface->CGameScriptInterface::
        SetHeroWillEnergyLevel(75.0f);
    if (g_pObservedComponent != &endComponent)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    pInterface->CGameScriptInterface::
        SetHeroWillEnergyLevel(90.0f);
    if (g_pObservedComponent != &endComponent)
        return Fail("direct sentinel");

    std::printf("FSE2_00899a10_TEST PASS\n");
    return 0;
}

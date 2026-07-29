#include <cstdio>

class CGSIHeroTeenager_Player;
class CGSIHeroTeenager_Node;

class CGSIHeroTeenager_Manager
{
public:
    CGSIHeroTeenager_Player* GetPlayer();
};

class CGSIHeroTeenager_Player
{
public:
    void* GetHero();
};

class CGSIHeroTeenager_Container
{
public:
    CGSIHeroTeenager_Node* LowerBound(
        const long* pKey);

    CGSIHeroTeenager_Node* m_pFirst;
    CGSIHeroTeenager_Node* m_pEnd;
};

class CGSIHeroTeenager_Morph
{
public:
    void SetAsTeenager(bool teenager);
};

class CGSIHeroTeenager_Stats
{
public:
    void ForceStaminaTo(float stamina);
    void SoberUp();
};

class CGSIHeroTeenager_Node
{
public:
    long m_Key;
    void* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual void SetHeroAsTeenager(
        bool teenager) const;
};

struct CGSIHeroTeenager_InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIHeroTeenager_Manager* m_pManager;
};

struct CGSIHeroTeenager_HeroStorage
{
    unsigned char m_Pad00[0x20];
    unsigned long m_Flags20;
    unsigned char m_Pad24[0x20];
    CGSIHeroTeenager_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIHeroTeenager_Player* g_pPlayer;
static CGSIHeroTeenager_HeroStorage* g_pHero;
static CGSIHeroTeenager_Node* g_pMorphResult;
static CGSIHeroTeenager_Node* g_pStatsResult;
static CGSIHeroTeenager_Container* g_pObservedContainer;
static CGSIHeroTeenager_Morph* g_pObservedMorph;
static CGSIHeroTeenager_Stats* g_pObservedStats;
static long g_ObservedKeys[2];
static float g_ObservedStamina;
static bool g_ObservedTeenager;
static unsigned long g_LowerBoundCalls;
static unsigned long g_MorphCalls;
static unsigned long g_ForceCalls;
static unsigned long g_SoberCalls;
static unsigned long g_Sequence;
static unsigned long g_ForceSequence;
static unsigned long g_SoberSequence;

CGSIHeroTeenager_Player*
CGSIHeroTeenager_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIHeroTeenager_Player::GetHero()
{
    return g_pHero;
}

CGSIHeroTeenager_Node*
CGSIHeroTeenager_Container::LowerBound(
    const long* pKey)
{
    g_pObservedContainer = this;
    if (g_LowerBoundCalls < 2)
        g_ObservedKeys[g_LowerBoundCalls] = *pKey;
    ++g_LowerBoundCalls;
    return *pKey == 3 ? g_pMorphResult : g_pStatsResult;
}

void CGSIHeroTeenager_Morph::SetAsTeenager(
    bool teenager)
{
    g_pObservedMorph = this;
    g_ObservedTeenager = teenager;
    ++g_MorphCalls;
}

void CGSIHeroTeenager_Stats::ForceStaminaTo(
    float stamina)
{
    g_pObservedStats = this;
    g_ObservedStamina = stamina;
    g_ForceSequence = ++g_Sequence;
    ++g_ForceCalls;
}

void CGSIHeroTeenager_Stats::SoberUp()
{
    g_pObservedStats = this;
    g_SoberSequence = ++g_Sequence;
    ++g_SoberCalls;
}

static void ResetObservations()
{
    g_pObservedContainer = 0;
    g_pObservedMorph = 0;
    g_pObservedStats = 0;
    g_ObservedKeys[0] = -1;
    g_ObservedKeys[1] = -1;
    g_ObservedStamina = 0.0f;
    g_ObservedTeenager = false;
    g_LowerBoundCalls = 0;
    g_MorphCalls = 0;
    g_ForceCalls = 0;
    g_SoberCalls = 0;
    g_Sequence = 0;
    g_ForceSequence = 0;
    g_SoberSequence = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_008995d0_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    CGSIHeroTeenager_InterfaceStorage
        interfaceStorage = {};
    CGSIHeroTeenager_Manager manager;
    CGSIHeroTeenager_Player player;
    CGSIHeroTeenager_HeroStorage hero = {};
    CGSIHeroTeenager_Morph foundMorph;
    CGSIHeroTeenager_Morph endMorph;
    CGSIHeroTeenager_Stats foundStats;
    CGSIHeroTeenager_Stats endStats;
    CGSIHeroTeenager_Node morphNode = {};
    CGSIHeroTeenager_Node statsNode = {};
    CGSIHeroTeenager_Node endNode = {};

    interfaceStorage.m_pManager = &manager;
    CGameScriptInterface* const pInterface =
        reinterpret_cast<CGameScriptInterface*>(
            &interfaceStorage);
    g_pPlayer = &player;
    morphNode.m_Key = 3;
    morphNode.m_pComponent = &foundMorph;
    statsNode.m_Key = 4;
    statsNode.m_pComponent = &foundStats;
    hero.m_Container.m_pEnd = &endNode;

    ResetObservations();
    g_pHero = 0;
    pInterface->CGameScriptInterface::
        SetHeroAsTeenager(true);
    if (g_MorphCalls != 0 || g_ForceCalls != 0)
        return Fail("null hero guard");

    g_pHero = &hero;
    ResetObservations();
    hero.m_Flags91 = 1;
    pInterface->CGameScriptInterface::
        SetHeroAsTeenager(true);
    if (g_MorphCalls != 0 || g_ForceCalls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0x08;
    g_pMorphResult = &morphNode;
    pInterface->CGameScriptInterface::
        SetHeroAsTeenager(true);
    if (g_pObservedContainer != &hero.m_Container ||
        g_ObservedKeys[0] != 3 ||
        g_pObservedMorph != &foundMorph ||
        !g_ObservedTeenager ||
        g_MorphCalls != 1 ||
        g_ForceCalls != 0 ||
        g_SoberCalls != 0)
        return Fail("morph-only true dispatch");

    ResetObservations();
    pInterface->CGameScriptInterface::
        SetHeroAsTeenager(false);
    if (g_pObservedMorph != &foundMorph ||
        g_ObservedTeenager)
        return Fail("morph-only false dispatch");

    ResetObservations();
    hero.m_Flags20 = 0x18;
    g_pMorphResult = &morphNode;
    g_pStatsResult = &statsNode;
    pInterface->CGameScriptInterface::
        SetHeroAsTeenager(true);
    if (g_ObservedKeys[0] != 3 ||
        g_ObservedKeys[1] != 4 ||
        g_pObservedMorph != &foundMorph ||
        g_pObservedStats != &foundStats ||
        g_ObservedStamina != 1.0f ||
        g_MorphCalls != 1 ||
        g_ForceCalls != 1 ||
        g_SoberCalls != 1 ||
        g_ForceSequence >= g_SoberSequence)
        return Fail("morph and stats sequence");

    ResetObservations();
    hero.m_Flags20 = 0x08;
    morphNode.m_Key = 4;
    endNode.m_pComponent = &endMorph;
    g_pMorphResult = &morphNode;
    pInterface->CGameScriptInterface::
        SetHeroAsTeenager(true);
    if (g_pObservedMorph != &endMorph)
        return Fail("greater-key morph sentinel");

    ResetObservations();
    g_pMorphResult = &endNode;
    pInterface->CGameScriptInterface::
        SetHeroAsTeenager(false);
    if (g_pObservedMorph != &endMorph)
        return Fail("direct morph sentinel");

    ResetObservations();
    hero.m_Flags20 = 0x18;
    morphNode.m_Key = 3;
    morphNode.m_pComponent = &foundMorph;
    statsNode.m_Key = 5;
    endNode.m_pComponent = &endStats;
    g_pMorphResult = &morphNode;
    g_pStatsResult = &statsNode;
    pInterface->CGameScriptInterface::
        SetHeroAsTeenager(true);
    if (g_pObservedStats != &endStats ||
        g_ForceCalls != 1 ||
        g_SoberCalls != 1)
        return Fail("greater-key stats sentinel");

    std::printf("FSE2_008995d0_TEST PASS\n");
    return 0;
}

#include <cstdio>

struct TransitionCall { int internalIndex; int unknown1; long themeGlobalIndex; float blendTime; };
static TransitionCall g_calls[16];
static int g_callCount = 0;
static long g_indexToReturn = 0;

class CCharString { public: int dummy; };

class CDefinitionManager
{
public:
    long GetDefGlobalIndexFromName(const CCharString& name) { (void)name; return g_indexToReturn; }
};

class CGameDefinitionManager : public CDefinitionManager
{
public:
    static CGameDefinitionManager& Get() { static CGameDefinitionManager s; return s; }
};

class CEnvironment
{
public:
    void TransitionToTheme(int internalIndex, int unknown1, long themeGlobalIndex, float blendTime)
    {
        TransitionCall c; c.internalIndex = internalIndex; c.unknown1 = unknown1; c.themeGlobalIndex = themeGlobalIndex; c.blendTime = blendTime;
        g_calls[g_callCount++] = c;
    }
};

struct CGameScriptInterfaceEnvironmentOwnerOverlay { char m_Pad00[0x18]; CEnvironment* m_pEnvironment; };
struct CGameScriptInterfaceOverlay { void* m_pVftable; CGameScriptInterfaceEnvironmentOwnerOverlay* m_pEnvironmentOwner; };

class CGameScriptInterface
{
public:
    void TransitionToThemeAllInternals(const CCharString& themeName, float blendTime) const;
};

void CGameScriptInterface::TransitionToThemeAllInternals(const CCharString& themeName, float blendTime) const
{
    const long globalIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(themeName);
    if (0 < globalIndex)
    {
        const CGameScriptInterfaceOverlay* const pThis = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);
        for (int internalIndex = 1; internalIndex <= 4; ++internalIndex)
        {
            pThis->m_pEnvironmentOwner->m_pEnvironment->TransitionToTheme(internalIndex, 1, globalIndex, blendTime);
        }
    }
}

int main()
{
    CEnvironment env;
    CGameScriptInterfaceEnvironmentOwnerOverlay owner;
    owner.m_pEnvironment = &env;
    CGameScriptInterfaceOverlay overlay;
    overlay.m_pVftable = 0;
    overlay.m_pEnvironmentOwner = &owner;

    CGameScriptInterface* pIface = reinterpret_cast<CGameScriptInterface*>(&overlay);
    CCharString nm; nm.dummy = 0;

    g_callCount = 0; g_indexToReturn = 77;
    pIface->TransitionToThemeAllInternals(nm, 2.5f);
    if (g_callCount != 4) { std::printf("BAD count1=%d\n", g_callCount); return 1; }
    for (int i = 0; i < 4; ++i)
    {
        if (g_calls[i].internalIndex != i + 1) { std::printf("BAD idx %d\n", i); return 1; }
        if (g_calls[i].unknown1 != 1) { std::printf("BAD unk %d\n", i); return 1; }
        if (g_calls[i].themeGlobalIndex != 77) { std::printf("BAD gi %d\n", i); return 1; }
        if (g_calls[i].blendTime != 2.5f) { std::printf("BAD blend %d\n", i); return 1; }
    }

    g_callCount = 0; g_indexToReturn = 0;
    pIface->TransitionToThemeAllInternals(nm, 1.0f);
    if (g_callCount != 0) { std::printf("BAD count2=%d\n", g_callCount); return 1; }

    g_callCount = 0; g_indexToReturn = -5;
    pIface->TransitionToThemeAllInternals(nm, 1.0f);
    if (g_callCount != 0) { std::printf("BAD count3=%d\n", g_callCount); return 1; }

    std::printf("THEME_TRANSITION_OK\n");
    return 0;
}
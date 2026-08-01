class CCharString;
class CDefinitionManager;
class CGameDefinitionManager;
class CEnvironment;

class CDefinitionManager
{
public:
    long GetDefGlobalIndexFromName(const CCharString& name);
};

class CGameDefinitionManager : public CDefinitionManager
{
public:
    static CGameDefinitionManager& Get();
};

class CEnvironment
{
public:
    void TransitionToTheme(
        int internalIndex,
        int unknown1,
        long themeGlobalIndex,
        float blendTime);
};

struct CGameScriptInterfaceEnvironmentOwnerOverlay
{
    char m_Pad00[0x18];
    CEnvironment* m_pEnvironment; // 0x18
};

struct CGameScriptInterfaceOverlay
{
    void* m_pVftable; // 0x00
    CGameScriptInterfaceEnvironmentOwnerOverlay* m_pEnvironmentOwner; // 0x04
};

class CGameScriptInterface
{
public:
    virtual void TransitionToThemeAllInternals(
        const CCharString& themeName,
        float blendTime) const;
};

void CGameScriptInterface::TransitionToThemeAllInternals(
    const CCharString& themeName,
    float blendTime) const
{
    const long globalIndex =
        CGameDefinitionManager::Get().GetDefGlobalIndexFromName(themeName);

    if (0 < globalIndex)
    {
        const CGameScriptInterfaceOverlay* const pThis =
            reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);

        for (int internalIndex = 1; internalIndex <= 4; ++internalIndex)
        {
            pThis->m_pEnvironmentOwner->m_pEnvironment->TransitionToTheme(
                internalIndex,
                1,
                globalIndex,
                blendTime);
        }
    }
}
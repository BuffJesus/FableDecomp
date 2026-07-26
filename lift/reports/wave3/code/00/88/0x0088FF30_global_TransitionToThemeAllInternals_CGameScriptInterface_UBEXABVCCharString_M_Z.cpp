#include <cstddef>
#include <cstdint>

class CCharString;
class CDefinitionManager;
class CGameDefinitionManager;
class CEnvironment;

class CDefinitionManager
{
public:
    long __thiscall GetDefGlobalIndexFromName(const CCharString& name);
};

class CGameDefinitionManager : public CDefinitionManager
{
public:
    static CGameDefinitionManager& __fastcall Get();
};

class CEnvironment
{
public:
    void __thiscall TransitionToTheme(
        std::int32_t internalIndex,
        std::int32_t unknown1,
        long themeGlobalIndex,
        float blendTime);
};

struct CGameScriptInterfaceEnvironmentOwnerOverlay
{
    std::byte m_Pad00[0x18];
    CEnvironment* m_pEnvironment; // 0x18
};

static_assert(offsetof(CGameScriptInterfaceEnvironmentOwnerOverlay, m_pEnvironment) == 0x18);

struct CGameScriptInterfaceOverlay
{
    void* m_pVftable; // 0x00
    CGameScriptInterfaceEnvironmentOwnerOverlay* m_pEnvironmentOwner; // 0x04
};

static_assert(offsetof(CGameScriptInterfaceOverlay, m_pEnvironmentOwner) == 0x04);

class CGameScriptInterface
{
public:
    virtual void __thiscall TransitionToThemeAllInternals(
        const CCharString& themeName,
        float blendTime) const;
};

void __thiscall CGameScriptInterface::TransitionToThemeAllInternals(
    const CCharString& themeName,
    float blendTime) const
{
    CGameDefinitionManager& definitionManager = CGameDefinitionManager::Get();
    const long globalIndex = definitionManager.GetDefGlobalIndexFromName(themeName);

    if (0 < globalIndex)
    {
        const auto* const pThis = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);

        for (std::int32_t internalIndex = 1; internalIndex < 5; ++internalIndex)
        {
            pThis->m_pEnvironmentOwner->m_pEnvironment->TransitionToTheme(
                internalIndex,
                1,
                globalIndex,
                blendTime);
        }
    }
}

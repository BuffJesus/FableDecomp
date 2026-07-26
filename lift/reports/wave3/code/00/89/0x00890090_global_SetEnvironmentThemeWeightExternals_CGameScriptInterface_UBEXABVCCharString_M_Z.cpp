#include <cstddef>
#include <cstdint>

class CCharString;
class CDefinitionManager;
class CEnvironment;

void* __cdecl GFGetPreMainMemoryUsed();

class CDefinitionManager
{
public:
    long __thiscall GetDefGlobalIndexFromName(const CCharString& name);
};

class CEnvironment
{
public:
    void __thiscall TransitionToTheme(
        std::int32_t internalIndex,
        std::int32_t unknown1,
        long themeGlobalIndex,
        float weight);
};

struct CGameScriptInterfaceEnvironmentOwnerOverlay
{
    std::byte m_Pad00[0x18];
    CEnvironment* m_pEnvironment; // 0x18
};
static_assert(offsetof(CGameScriptInterfaceEnvironmentOwnerOverlay, m_pEnvironment) == 0x18);

struct CGameScriptInterfaceOverlay
{
    std::byte m_Pad00[0x04];
    CGameScriptInterfaceEnvironmentOwnerOverlay* m_pEnvironmentOwner; // 0x04
};
static_assert(offsetof(CGameScriptInterfaceOverlay, m_pEnvironmentOwner) == 0x04);

class CGameScriptInterface
{
public:
    void __thiscall SetEnvironmentThemeWeightExternals(
        const CCharString& themeName,
        float weight) const
    {
        auto* const pDefinitionManager =
            static_cast<CDefinitionManager*>(GFGetPreMainMemoryUsed());
        const long themeGlobalIndex =
            pDefinitionManager->GetDefGlobalIndexFromName(themeName);

        if (0 < themeGlobalIndex)
        {
            const auto* const pThis =
                reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);
            pThis->m_pEnvironmentOwner->m_pEnvironment->TransitionToTheme(
                0,
                1,
                themeGlobalIndex,
                weight);
        }
    }
};
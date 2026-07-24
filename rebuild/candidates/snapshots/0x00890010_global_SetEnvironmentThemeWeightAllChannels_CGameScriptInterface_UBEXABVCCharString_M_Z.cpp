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
    void __thiscall SetThemeWeightAllChannels(
        int channelIndex,
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
    void* m_pVftable; // 0x00
    CGameScriptInterfaceEnvironmentOwnerOverlay* m_pEnvironmentOwner; // 0x04
};

static_assert(offsetof(CGameScriptInterfaceOverlay, m_pEnvironmentOwner) == 0x04);

class CGameScriptInterface
{
public:
    virtual void __thiscall SetEnvironmentThemeWeightAllChannels(
        const CCharString& themeName,
        float weight) const;
};

void __thiscall CGameScriptInterface::SetEnvironmentThemeWeightAllChannels(
    const CCharString& themeName,
    float weight) const
{
    CDefinitionManager* const pDefinitionManager =
        static_cast<CDefinitionManager*>(GFGetPreMainMemoryUsed());
    const long globalIndex = pDefinitionManager->GetDefGlobalIndexFromName(themeName);

    if (0 < globalIndex)
    {
        const auto* const pThis = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);
        pThis->m_pEnvironmentOwner->m_pEnvironment->SetThemeWeightAllChannels(
            1,
            globalIndex,
            weight);
    }
}
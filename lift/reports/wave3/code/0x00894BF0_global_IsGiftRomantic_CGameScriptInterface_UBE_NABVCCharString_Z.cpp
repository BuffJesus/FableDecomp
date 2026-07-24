#include <cstddef>
#include <cstdint>

class CCharString;
class CDefinitionManager;
class CGameScriptInterface;
class CDefClassBase;
class CSpecialAbilitiesDrunkennessDef;
class CAICreatureWillPowerIndicatorDef;

template <typename T>
struct CDefPointer
{
    T* m_pObject; // 0x00
};

static_assert(sizeof(CDefPointer<CSpecialAbilitiesDrunkennessDef const>) == 0x04);
static_assert(sizeof(CDefPointer<CAICreatureWillPowerIndicatorDef const>) == 0x04);

class CDefinitionManager
{
public:
    long __thiscall GetDefGlobalIndexFromName(const CCharString* pName);

    template <typename T>
    static bool __thiscall GetDef(
        CDefinitionManager* pManager,
        long defIndex,
        CDefPointer<T const>* pDef);
};

class CDefClassBase
{
public:
    template <typename T>
    bool __thiscall GetSubDef(CDefPointer<T const>* pDef);
};

class CGameScriptInterface
{
public:
    bool __thiscall IsGiftRomantic(const CCharString& giftType) const;
};

extern "C" CDefinitionManager* __cdecl GFGetPreMainMemoryUsed();

namespace
{
struct CGameScriptInterfaceOverlay
{
    std::byte m_Pad00[0x10];
    CDefinitionManager* m_pDefinitionManager; // 0x10
};

static_assert(offsetof(CGameScriptInterfaceOverlay, m_pDefinitionManager) == 0x10);

struct CRefCountedDefOverlay
{
    void** m_pVftable;       // 0x00
    std::int32_t m_RefCount; // 0x04
};

static_assert(offsetof(CRefCountedDefOverlay, m_pVftable) == 0x00);
static_assert(offsetof(CRefCountedDefOverlay, m_RefCount) == 0x04);

struct CAICreatureWillPowerIndicatorDefOverlay
{
    std::byte m_Pad00[0x28];
    std::int32_t m_Value28; // 0x28
};

static_assert(offsetof(CAICreatureWillPowerIndicatorDefOverlay, m_Value28) == 0x28);
}

bool __thiscall CGameScriptInterface::IsGiftRomantic(const CCharString& giftType) const
{
    const auto* const pThis = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);
    const long defIndex = pThis->m_pDefinitionManager->GetDefGlobalIndexFromName(&giftType);

    CDefPointer<CSpecialAbilitiesDrunkennessDef const> drunkennessDef{};
    CDefPointer<CAICreatureWillPowerIndicatorDef const> willPowerIndicatorDef{};

    CDefinitionManager* const pPreMainDefinitionManager = GFGetPreMainMemoryUsed();
    const bool foundDrunkennessDef =
        CDefinitionManager::GetDef<CSpecialAbilitiesDrunkennessDef>(
            pPreMainDefinitionManager,
            defIndex,
            &drunkennessDef);

    CDefClassBase* const pDrunkennessBase =
        reinterpret_cast<CDefClassBase*>(const_cast<CSpecialAbilitiesDrunkennessDef*>(drunkennessDef.m_pObject));

    if (foundDrunkennessDef)
    {
        const bool foundWillPowerIndicator =
            pDrunkennessBase->GetSubDef<CAICreatureWillPowerIndicatorDef>(&willPowerIndicatorDef);

        if (foundWillPowerIndicator)
        {
            const std::int32_t value28 =
                reinterpret_cast<const CAICreatureWillPowerIndicatorDefOverlay*>(willPowerIndicatorDef.m_pObject)
                    ->m_Value28;

            CRefCountedDefOverlay* const pWillPowerOverlay =
                reinterpret_cast<CRefCountedDefOverlay*>(
                    const_cast<CAICreatureWillPowerIndicatorDef*>(willPowerIndicatorDef.m_pObject));
            const std::int32_t willPowerRefCount = pWillPowerOverlay->m_RefCount;
            pWillPowerOverlay->m_RefCount = willPowerRefCount + -1;
            if (willPowerRefCount + -1 == 0)
            {
                reinterpret_cast<void(__thiscall*)(void*)>(pWillPowerOverlay->m_pVftable[1])(
                    willPowerIndicatorDef.m_pObject);
            }

            if (pDrunkennessBase != nullptr)
            {
                CRefCountedDefOverlay* const pDrunkennessOverlay =
                    reinterpret_cast<CRefCountedDefOverlay*>(pDrunkennessBase);
                pDrunkennessOverlay->m_RefCount = pDrunkennessOverlay->m_RefCount + -1;
                if (pDrunkennessOverlay->m_RefCount == 0)
                {
                    reinterpret_cast<void(__thiscall*)(void*)>(pDrunkennessOverlay->m_pVftable[1])(pDrunkennessBase);
                }
            }

            return value28 == 1;
        }

        if (willPowerIndicatorDef.m_pObject != nullptr)
        {
            CRefCountedDefOverlay* const pWillPowerOverlay =
                reinterpret_cast<CRefCountedDefOverlay*>(
                    const_cast<CAICreatureWillPowerIndicatorDef*>(willPowerIndicatorDef.m_pObject));
            pWillPowerOverlay->m_RefCount = pWillPowerOverlay->m_RefCount + -1;
            if (pWillPowerOverlay->m_RefCount == 0)
            {
                reinterpret_cast<void(__thiscall*)(void*)>(pWillPowerOverlay->m_pVftable[1])(
                    willPowerIndicatorDef.m_pObject);
            }
        }
    }

    if (pDrunkennessBase != nullptr)
    {
        CRefCountedDefOverlay* const pDrunkennessOverlay =
            reinterpret_cast<CRefCountedDefOverlay*>(pDrunkennessBase);
        pDrunkennessOverlay->m_RefCount = pDrunkennessOverlay->m_RefCount + -1;
        if (pDrunkennessOverlay->m_RefCount == 0)
        {
            reinterpret_cast<void(__thiscall*)(void*)>(pDrunkennessOverlay->m_pVftable[1])(pDrunkennessBase);
        }
    }

    return false;
}
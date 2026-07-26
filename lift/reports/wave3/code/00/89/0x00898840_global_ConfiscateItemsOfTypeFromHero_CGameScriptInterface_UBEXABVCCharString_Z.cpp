#include <cstddef>
#include <cstdint>

class CCharString;
class CDefinitionManager;
class CGameScriptInterface;
class CTCBase;

enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    static void* __thiscall LowerBound(CVectorMap* pThis, const TKey* pKey);
};

class CDefinitionManager
{
public:
    long __thiscall GetDefGlobalIndexFromName(const CCharString* name);
};

namespace NInventory
{
class CTCInventoryBase
{
public:
    void __thiscall ConfiscateAllItemsOfType(long definitionIndex);
};
}

extern "C" CDefinitionManager* __cdecl GFGetPreMainMemoryUsed();

class CGameScriptInterface
{
public:
    void __thiscall ConfiscateItemsOfTypeFromHero(const CCharString& itemType) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_ConfiscateItemsOfTypeFromHero_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(
        CGameScriptInterface_ConfiscateItemsOfTypeFromHero_SelfOverlay,
        m_Field14) == 0x14);

struct CGameScriptInterface_ConfiscateItemsOfTypeFromHero_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_ConfiscateItemsOfTypeFromHero_TCInterfaceEntryOverlay,
        m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_ConfiscateItemsOfTypeFromHero_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_ConfiscateItemsOfTypeFromHero_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint32_t m_Flags20; // 0x20
    std::byte m_Pad24[0x20];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_ConfiscateItemsOfTypeFromHero_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(
        CGameScriptInterface_ConfiscateItemsOfTypeFromHero_TargetOverlay,
        m_Flags20) == 0x20);
static_assert(
    offsetof(
        CGameScriptInterface_ConfiscateItemsOfTypeFromHero_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_ConfiscateItemsOfTypeFromHero_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(
        CGameScriptInterface_ConfiscateItemsOfTypeFromHero_TargetOverlay,
        m_Flags91) == 0x91);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC0 =
    CGameScriptInterface_ConfiscateItemsOfTypeFromHero_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);
}

void __thiscall CGameScriptInterface::ConfiscateItemsOfTypeFromHero(
    const CCharString& itemType) const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_ConfiscateItemsOfTypeFromHero_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DC0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        CDefinitionManager* const definitionManager = GFGetPreMainMemoryUsed();
        const long definitionIndex = definitionManager->GetDefGlobalIndexFromName(&itemType);
        if (0 < definitionIndex)
        {
            if ((target->m_Flags20 & 0x00020000) != 0)
            {
                std::int32_t interfaceTypeValue = 0x11;

                auto* pEntry =
                    reinterpret_cast<
                        CGameScriptInterface_ConfiscateItemsOfTypeFromHero_TCInterfaceEntryOverlay*>(
                        TCInterfaceMap::LowerBound(
                            reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                            reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

                if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x11 < pEntry->m_Key))
                {
                    pEntry = target->m_pTCInterfaceMapEnd;
                }

                reinterpret_cast<NInventory::CTCInventoryBase*>(pEntry->m_pInterface)
                    ->ConfiscateAllItemsOfType(definitionIndex);
            }

            if ((target->m_Flags20 & 0x00080000) != 0)
            {
                std::int32_t interfaceTypeValue = 0x13;

                auto* pEntry =
                    reinterpret_cast<
                        CGameScriptInterface_ConfiscateItemsOfTypeFromHero_TCInterfaceEntryOverlay*>(
                        TCInterfaceMap::LowerBound(
                            reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                            reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

                if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x13 < pEntry->m_Key))
                {
                    pEntry = target->m_pTCInterfaceMapEnd;
                }

                reinterpret_cast<NInventory::CTCInventoryBase*>(pEntry->m_pInterface)
                    ->ConfiscateAllItemsOfType(definitionIndex);
            }

            if ((target->m_Flags20 & 0x00040000) != 0)
            {
                std::int32_t interfaceTypeValue = 0x12;

                auto* pEntry =
                    reinterpret_cast<
                        CGameScriptInterface_ConfiscateItemsOfTypeFromHero_TCInterfaceEntryOverlay*>(
                        TCInterfaceMap::LowerBound(
                            reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                            reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

                if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x12 < pEntry->m_Key))
                {
                    pEntry = target->m_pTCInterfaceMapEnd;
                }

                reinterpret_cast<NInventory::CTCInventoryBase*>(pEntry->m_pInterface)
                    ->ConfiscateAllItemsOfType(definitionIndex);
            }
        }
    }
}
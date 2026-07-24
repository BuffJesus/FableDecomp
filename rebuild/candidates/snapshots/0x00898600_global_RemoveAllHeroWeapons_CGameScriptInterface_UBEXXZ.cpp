#include <cstddef>
#include <cstdint>
#include <cstdlib>

class CGameScriptInterface;
class CTCBase;

enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    static void* __thiscall LowerBound(
        CVectorMap* pThis,
        const TKey* pKey);
};

namespace NInventory
{
class CTCInventoryBase
{
public:
    void __thiscall GetAllItems(void* pItems);
};
}

class CGameScriptInterface
{
public:
    void __thiscall RemoveAllHeroWeapons() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_RemoveAllHeroWeapons_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_RemoveAllHeroWeapons_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_RemoveAllHeroWeapons_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_RemoveAllHeroWeapons_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_RemoveAllHeroWeapons_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint32_t m_Flags20; // 0x20
    std::byte m_Pad24[0x20];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_RemoveAllHeroWeapons_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(CGameScriptInterface_RemoveAllHeroWeapons_TargetOverlay, m_Flags20) == 0x20);
static_assert(
    offsetof(
        CGameScriptInterface_RemoveAllHeroWeapons_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_RemoveAllHeroWeapons_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(CGameScriptInterface_RemoveAllHeroWeapons_TargetOverlay, m_Flags91) == 0x91);

struct InventoryItemPair
{
    std::int32_t m_First;  // 0x00
    std::int32_t m_Second; // 0x04
};
static_assert(offsetof(InventoryItemPair, m_First) == 0x00);
static_assert(offsetof(InventoryItemPair, m_Second) == 0x04);
static_assert(sizeof(InventoryItemPair) == 0x08);

struct InventoryItemVectorOverlay
{
    InventoryItemPair* m_pBegin;    // 0x00
    InventoryItemPair* m_pEnd;      // 0x04
    InventoryItemPair* m_pCapacity; // 0x08
};
static_assert(offsetof(InventoryItemVectorOverlay, m_pBegin) == 0x00);
static_assert(offsetof(InventoryItemVectorOverlay, m_pEnd) == 0x04);
static_assert(offsetof(InventoryItemVectorOverlay, m_pCapacity) == 0x08);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC0 = CGameScriptInterface_RemoveAllHeroWeapons_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);
}

void __thiscall CGameScriptInterface::RemoveAllHeroWeapons() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_RemoveAllHeroWeapons_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DC0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags20 & 0x00080000) != 0))
    {
        ETCInterfaceType interfaceTypeValue = static_cast<ETCInterfaceType>(0x13);

        auto* pEntry =
            reinterpret_cast<CGameScriptInterface_RemoveAllHeroWeapons_TCInterfaceEntryOverlay*>(
                TCInterfaceMap::LowerBound(
                    reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                    &interfaceTypeValue));

        if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x13 < pEntry->m_Key))
        {
            pEntry = target->m_pTCInterfaceMapEnd;
        }

        auto* const pInventory =
            reinterpret_cast<NInventory::CTCInventoryBase*>(pEntry->m_pInterface);

        InventoryItemVectorOverlay items{};
        pInventory->GetAllItems(&items);

        for (InventoryItemPair* pItem = items.m_pBegin; pItem != items.m_pEnd; ++pItem)
        {
            using RawRemoveItemFn =
                void(__thiscall*)(NInventory::CTCInventoryBase*, std::int32_t, std::int32_t);

            auto* const vftable = *reinterpret_cast<void***>(pInventory);
            reinterpret_cast<RawRemoveItemFn>(vftable[0x13C / sizeof(void*)])(
                pInventory,
                pItem->m_First,
                pItem->m_Second);
        }

        using RawFinalizeFn = void(__thiscall*)(NInventory::CTCInventoryBase*);
        auto* const vftable = *reinterpret_cast<void***>(pInventory);
        reinterpret_cast<RawFinalizeFn>(vftable[0x128 / sizeof(void*)])(pInventory);

        if (items.m_pBegin != nullptr)
        {
            std::free(items.m_pBegin);
        }
    }
}
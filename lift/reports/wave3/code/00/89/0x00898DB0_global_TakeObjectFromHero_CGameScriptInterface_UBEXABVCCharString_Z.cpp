#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

class CCharString;
class CDefinitionManager;
class CGameScriptInterface;
class CTCBase;

enum ETCInterfaceType : std::int32_t
{
};

class CDefinitionManager
{
public:
    long __thiscall GetDefGlobalIndexFromName(const CCharString* pName);
};

class CTCInventoryItem
{
public:
    static ETCInterfaceType __fastcall GetInventoryCategory(long definitionIndex);
};

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue>
struct CKeyPair
{
    TKey m_Key;     // 0x00
    TValue m_Value; // 0x04
};

using InventoryInterfacePair = CKeyPair<ETCInterfaceType, CTCBase*>;
static_assert(offsetof(InventoryInterfacePair, m_Key) == 0x00);
static_assert(offsetof(InventoryInterfacePair, m_Value) == 0x04);
static_assert(sizeof(InventoryInterfacePair) == 0x08);

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    CKeyPair<TKey, TValue>* m_pBegin; // 0x00
    CKeyPair<TKey, TValue>* m_pEnd;   // 0x04

    CKeyPair<TKey, TValue>* __thiscall LowerBound(const TKey* pKey);
};

namespace NInventory
{
class CTCInventoryBase
{
};
} // namespace NInventory

class CGameScriptInterface
{
public:
    void __thiscall TakeObjectFromHero(const CCharString& objectName) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterfaceTakeObjectFromHeroSelfOverlay
{
    std::byte m_Pad00[0x10];
    CDefinitionManager* m_pDefinitionManager; // 0x10
    void* m_Field14;                          // 0x14
};

static_assert(
    offsetof(CGameScriptInterfaceTakeObjectFromHeroSelfOverlay, m_pDefinitionManager) == 0x10);
static_assert(offsetof(CGameScriptInterfaceTakeObjectFromHeroSelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterfaceTakeObjectFromHeroTargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint32_t m_InterfaceFlags20[9]; // 0x20
    TCInterfaceMap m_Map44;              // 0x44
};

static_assert(
    offsetof(CGameScriptInterfaceTakeObjectFromHeroTargetOverlay, m_InterfaceFlags20) == 0x20);
static_assert(offsetof(CGameScriptInterfaceTakeObjectFromHeroTargetOverlay, m_Map44) == 0x44);

using Helper00449970Fn = void* (__thiscall*)(void*);
using Helper00487DC0Fn = CGameScriptInterfaceTakeObjectFromHeroTargetOverlay* (__thiscall*)(void*);
using GetNumberOfItemsRawFn = long (__thiscall*)(NInventory::CTCInventoryBase*, long);
using RemoveOneRawFn = void (__thiscall*)(NInventory::CTCInventoryBase*, long, int);

inline const Helper00449970Fn CALL_00449970 =
    reinterpret_cast<Helper00449970Fn>(0x00449970);
inline const Helper00487DC0Fn CALL_00487DC0 =
    reinterpret_cast<Helper00487DC0Fn>(0x00487DC0);
inline const GetNumberOfItemsRawFn CALL_005BDF08 =
    reinterpret_cast<GetNumberOfItemsRawFn>(0x005BDF08);
} // namespace

void __thiscall CGameScriptInterface::TakeObjectFromHero(const CCharString& objectName) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterfaceTakeObjectFromHeroSelfOverlay*>(this);

    const long definitionIndex =
        self->m_pDefinitionManager->GetDefGlobalIndexFromName(&objectName);
    if (definitionIndex <= 0)
    {
        return;
    }

    const ETCInterfaceType inventoryCategory =
        CTCInventoryItem::GetInventoryCategory(definitionIndex);
    if (inventoryCategory == static_cast<ETCInterfaceType>(0))
    {
        return;
    }

    auto* const target = CALL_00487DC0(CALL_00449970(self->m_Field14));
    auto* pInventory = static_cast<NInventory::CTCInventoryBase*>(nullptr);

    if (static_cast<std::uint32_t>(inventoryCategory) >= 0x112u)
    {
        std::puts("bitset");
        std::abort();
    }

    const std::uint32_t mask =
        1u << (static_cast<std::uint32_t>(inventoryCategory) & 0x1Fu);
    const std::uint32_t wordIndex =
        static_cast<std::uint32_t>(inventoryCategory) >> 5;

    if ((target->m_InterfaceFlags20[wordIndex] & mask) != 0)
    {
        ETCInterfaceType searchKey = inventoryCategory;
        CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
            target->m_Map44.LowerBound(&searchKey);

        if ((pEntry == target->m_Map44.m_pEnd) ||
            (static_cast<std::int32_t>(inventoryCategory) < pEntry->m_Key))
        {
            pEntry = target->m_Map44.m_pEnd;
        }

        pInventory = reinterpret_cast<NInventory::CTCInventoryBase*>(pEntry->m_Value);
    }

    const long numberOfItems = CALL_005BDF08(pInventory, definitionIndex);
    if (numberOfItems > 0)
    {
        void** const vftable = *reinterpret_cast<void***>(pInventory);
        const auto removeOne =
            reinterpret_cast<RemoveOneRawFn>(vftable[0x13C / sizeof(void*)]);
        removeOne(pInventory, definitionIndex, 1);
    }
}

#include <cstddef>
#include <cstdint>

class CCharString;
class CComponent;
class CDefinitionManager;
class CGameScriptInterface;
class CTCBase;
class CTCInventoryWeapons;

namespace NInventory
{
class CTCInventoryBase
{
public:
    long __thiscall GetNumberOfItemsOfTypeInInventory(long definitionIndex);
};
} // namespace NInventory

enum ETCInterfaceType : std::int32_t
{
};

class CDefinitionManager
{
public:
    long __thiscall GetDefGlobalIndexFromName(const CCharString* pName);
};

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue>
struct CKeyPair
{
    TKey m_Key;     // 0x00
    TValue m_Value; // 0x04
};

static_assert(offsetof(CKeyPair<ETCInterfaceType, CTCBase*>, m_Key) == 0x00);
static_assert(offsetof(CKeyPair<ETCInterfaceType, CTCBase*>, m_Value) == 0x04);
static_assert(sizeof(CKeyPair<ETCInterfaceType, CTCBase*>) == 0x08);

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    CKeyPair<TKey, TValue>* m_pBegin; // 0x00
    CKeyPair<TKey, TValue>* m_pEnd;   // 0x04

    CKeyPair<TKey, TValue>* __thiscall LowerBound(const TKey* pKey);
};

class CTCInventoryWeapons
{
public:
    void __thiscall SetThingAsActiveWeapon(long definitionIndex, CComponent* pComponent);
};

class CGameScriptInterface
{
public:
    void __thiscall SetWeaponAsHerosActiveWeapon(const CCharString& weaponName) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterfaceOverlay
{
    std::byte m_Pad00[0x10];
    CDefinitionManager* m_pDefinitionManager; // 0x10
    void* m_Field14;                          // 0x14
};

static_assert(offsetof(CGameScriptInterfaceOverlay, m_pDefinitionManager) == 0x10);
static_assert(offsetof(CGameScriptInterfaceOverlay, m_Field14) == 0x14);

struct HeroThingOverlay
{
    std::byte m_Pad00[0x20];
    std::uint32_t m_Flags20; // 0x20
    std::byte m_Pad24[0x20];
    TCInterfaceMap m_Map44;  // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91;  // 0x91
};

static_assert(offsetof(HeroThingOverlay, m_Flags20) == 0x20);
static_assert(offsetof(HeroThingOverlay, m_Map44) == 0x44);
static_assert(offsetof(HeroThingOverlay, m_Flags91) == 0x91);

using Helper00449970Fn = void* (__thiscall*)(void*);
using Helper00487DC0Fn = HeroThingOverlay* (__thiscall*)(void*);

inline constexpr auto Helper00449970 =
    reinterpret_cast<Helper00449970Fn>(0x00449970);
inline constexpr auto Helper00487DC0 =
    reinterpret_cast<Helper00487DC0Fn>(0x00487DC0);
} // namespace

void __thiscall CGameScriptInterface::SetWeaponAsHerosActiveWeapon(
    const CCharString& weaponName) const
{
    const auto* const self = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);

    const long definitionIndex =
        self->m_pDefinitionManager->GetDefGlobalIndexFromName(&weaponName);
    if (definitionIndex <= 0)
    {
        return;
    }

    auto* const hero = Helper00487DC0(Helper00449970(self->m_Field14));
    if ((hero == nullptr) ||
        ((hero->m_Flags91 & 0x01) != 0) ||
        ((hero->m_Flags20 & 0x00080000U) == 0))
    {
        return;
    }

    ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x13);
    CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
        hero->m_Map44.LowerBound(&interfaceType);

    if ((pEntry == hero->m_Map44.m_pEnd) ||
        (static_cast<ETCInterfaceType>(0x13) < pEntry->m_Key))
    {
        pEntry = hero->m_Map44.m_pEnd;
    }

    auto* const pInventoryBase =
        reinterpret_cast<NInventory::CTCInventoryBase*>(pEntry->m_Value);
    if (pInventoryBase->GetNumberOfItemsOfTypeInInventory(definitionIndex) > 0)
    {
        reinterpret_cast<CTCInventoryWeapons*>(pInventoryBase)
            ->SetThingAsActiveWeapon(definitionIndex, nullptr);
    }
}
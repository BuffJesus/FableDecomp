#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CThing;

enum ETCInterfaceType : std::int32_t
{
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

class CGameScriptInterface
{
public:
    void __thiscall GiveHeroItem(const CScriptThing& item) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterfaceSelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};

static_assert(offsetof(CGameScriptInterfaceSelfOverlay, m_Field14) == 0x14);

struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    CThing* (__thiscall* GetThing)(const CScriptThing* pThis); // 0x2C
    std::byte m_Pad30[0xFC];
    bool (__thiscall* IsValidThing)(const CScriptThing* pThis); // 0x12C
};

static_assert(offsetof(CScriptThingVTableOverlay, GetThing) == 0x2C);
static_assert(offsetof(CScriptThingVTableOverlay, IsValidThing) == 0x12C);

struct CScriptThingOverlay
{
    const CScriptThingVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_pVTable) == 0x00);

struct HeroThingOverlay
{
    std::byte m_Pad00[0x38];
    std::uint32_t m_Flags38; // 0x38
    std::byte m_Pad3C[0x08];
    TCInterfaceMap m_InterfaceMap44; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(HeroThingOverlay, m_Flags38) == 0x38);
static_assert(offsetof(HeroThingOverlay, m_InterfaceMap44) == 0x44);
static_assert(offsetof(HeroThingOverlay, m_Flags91) == 0x91);

using Helper0040D959Fn = void* (__thiscall*)(void*);
using Helper00487DC0Fn = HeroThingOverlay* (__thiscall*)(void*);
using AddItemToInventoryFn = void (__thiscall*)(void* pThis, CThing* pThing, int unknownZero);

inline constexpr auto Helper0040D959 =
    reinterpret_cast<Helper0040D959Fn>(0x0040D959);
inline constexpr auto Helper00487DC0 =
    reinterpret_cast<Helper00487DC0Fn>(0x00487DC0);
inline constexpr auto AddItemToInventory =
    reinterpret_cast<AddItemToInventoryFn>(0x007CAB30);
} // namespace

void __thiscall CGameScriptInterface::GiveHeroItem(const CScriptThing& item) const
{
    const auto* const itemOverlay = reinterpret_cast<const CScriptThingOverlay*>(&item);
    if (!itemOverlay->m_pVTable->IsValidThing(&item))
    {
        return;
    }

    const auto* const self = reinterpret_cast<const CGameScriptInterfaceSelfOverlay*>(this);
    auto* const heroThing = Helper00487DC0(Helper0040D959(self->m_Field14));
    if ((heroThing == nullptr) || ((heroThing->m_Flags91 & 0x01) != 0))
    {
        return;
    }

    void* pAddItemTarget = const_cast<CScriptThing*>(&item);
    if ((heroThing->m_Flags38 & 0x00000800) != 0)
    {
        ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0xCB);
        CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
            heroThing->m_InterfaceMap44.LowerBound(&interfaceType);

        if ((pEntry == heroThing->m_InterfaceMap44.m_pEnd) ||
            (static_cast<ETCInterfaceType>(0xCB) < pEntry->m_Key))
        {
            pEntry = heroThing->m_InterfaceMap44.m_pEnd;
        }

        pAddItemTarget = pEntry->m_Value;
    }

    AddItemToInventory(pAddItemTarget, itemOverlay->m_pVTable->GetThing(&item), 0);
}
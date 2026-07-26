#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CTCCarrying;
class CThing;

enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    void* __thiscall LowerBound(const TKey* pKey);
};

class CTCCarrying
{
public:
    CThing* __thiscall GetThingInPrimarySlot();
};

class CGameScriptInterface
{
public:
    std::uint32_t __thiscall IsEntityWieldingWeapon(const CScriptThing& scriptThing) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct TCInterfaceEntryOverlay
{
    ETCInterfaceType m_Key; // 0x00
    CTCBase* m_pInterface;  // 0x04
};

static_assert(offsetof(TCInterfaceEntryOverlay, m_Key) == 0x00);
static_assert(offsetof(TCInterfaceEntryOverlay, m_pInterface) == 0x04);
static_assert(sizeof(TCInterfaceEntryOverlay) == 0x08);

struct TargetOverlay
{
    std::byte m_Pad00[0x28];
    std::uint8_t m_Flags28; // 0x28
    std::byte m_Pad29[0x1B];
    TCInterfaceMap m_TCInterfaceMap;      // 0x44
    TCInterfaceEntryOverlay* m_pMapEnd;   // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(TargetOverlay, m_Flags28) == 0x28);
static_assert(offsetof(TargetOverlay, m_TCInterfaceMap) == 0x44);
static_assert(offsetof(TargetOverlay, m_pMapEnd) == 0x48);
static_assert(offsetof(TargetOverlay, m_Flags91) == 0x91);

struct ThingOverlay
{
    std::byte m_Pad00[0x28];
    std::uint32_t m_Flags28; // 0x28
};

static_assert(offsetof(ThingOverlay, m_Flags28) == 0x28);

using RawScriptThingCall = TargetOverlay* (__thiscall*)(const CScriptThing*);
}

std::uint32_t __thiscall CGameScriptInterface::IsEntityWieldingWeapon(
    const CScriptThing& scriptThing) const
{
    const auto* const vtable = *reinterpret_cast<RawScriptThingCall* const*>(&scriptThing);
    TargetOverlay* const pThing = vtable[0x2c / sizeof(void*)](&scriptThing);

    if ((pThing == nullptr) ||
        ((pThing->m_Flags91 & 0x01u) != 0) ||
        ((pThing->m_Flags28 & 0x40u) == 0))
    {
        return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(pThing)) & 0xFFFFFF00u;
    }

    ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x46);
    auto* pEntry = reinterpret_cast<TCInterfaceEntryOverlay*>(
        pThing->m_TCInterfaceMap.LowerBound(&interfaceType));

    if ((pEntry == pThing->m_pMapEnd) || (static_cast<ETCInterfaceType>(0x46) < pEntry->m_Key))
    {
        pEntry = pThing->m_pMapEnd;
    }

    CThing* const pPrimaryThing =
        reinterpret_cast<CTCCarrying*>(pEntry->m_pInterface)->GetThingInPrimarySlot();

    if ((pPrimaryThing != nullptr) &&
        ((reinterpret_cast<ThingOverlay*>(pPrimaryThing)->m_Flags28 & 0x100u) != 0))
    {
        return 1;
    }

    return 0;
}
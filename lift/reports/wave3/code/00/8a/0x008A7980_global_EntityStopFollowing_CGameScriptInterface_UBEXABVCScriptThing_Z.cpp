#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CTCFollowed;
class CThing;
class CThingAICreature;

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

class CTCFollowed
{
public:
    bool __thiscall IsAIFollower(CThingAICreature* pFollower);
    void __thiscall RemoveFollower(CThing* pFollower);
};

class CGameScriptInterface
{
public:
    void __thiscall EntityStopFollowing(const CScriptThing& entity) const;
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
    void* (__thiscall* GetThing)(const CScriptThing* pThis); // 0x2C
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
    std::byte m_Pad00[0x34];
    std::uint32_t m_Flags34; // 0x34
    std::byte m_Pad38[0x0C];
    TCInterfaceMap m_InterfaceMap44; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(HeroThingOverlay, m_Flags34) == 0x34);
static_assert(offsetof(HeroThingOverlay, m_InterfaceMap44) == 0x44);
static_assert(offsetof(HeroThingOverlay, m_Flags91) == 0x91);

struct FollowerThingOverlay
{
    std::byte m_Pad00[0x28];
    std::uint8_t m_Flags28; // 0x28
    std::byte m_Pad29[0x1B];
    TCInterfaceMap m_InterfaceMap44; // 0x44
};

static_assert(offsetof(FollowerThingOverlay, m_Flags28) == 0x28);
static_assert(offsetof(FollowerThingOverlay, m_InterfaceMap44) == 0x44);

struct EntityEventsInterfaceOverlay
{
    std::byte m_Pad00[0x0C];
    std::byte m_EventList0C[0x08]; // opaque std::list at 0x0C
};

static_assert(offsetof(EntityEventsInterfaceOverlay, m_EventList0C) == 0x0C);

using Helper00449970Fn = void* (__thiscall*)(void*);
using Helper00487DC0Fn = HeroThingOverlay* (__thiscall*)(void*);
using RawThingCall006D5AB0Fn = void (__thiscall*)(CThing*);
using RemoveIfEventTypeFn = void (__thiscall*)(void*, const std::int32_t*);

inline constexpr auto Helper00449970 =
    reinterpret_cast<Helper00449970Fn>(0x00449970);
inline constexpr auto Helper00487DC0 =
    reinterpret_cast<Helper00487DC0Fn>(0x00487DC0);
inline constexpr auto RawThingCall006D5AB0 =
    reinterpret_cast<RawThingCall006D5AB0Fn>(0x006D5AB0);
inline constexpr auto RemoveIfEventType =
    reinterpret_cast<RemoveIfEventTypeFn>(0x008AC860);
}

void __thiscall CGameScriptInterface::EntityStopFollowing(const CScriptThing& entity) const
{
    const auto* const entityOverlay = reinterpret_cast<const CScriptThingOverlay*>(&entity);
    if (!entityOverlay->m_pVTable->IsValidThing(&entity))
    {
        return;
    }

    void* const pResolvedEntityThing = entityOverlay->m_pVTable->GetThing(&entity);

    const auto* const self = reinterpret_cast<const CGameScriptInterfaceSelfOverlay*>(this);
    auto* const pHeroThing = Helper00487DC0(Helper00449970(self->m_Field14));
    if ((pHeroThing == nullptr) ||
        ((pHeroThing->m_Flags91 & 0x01) != 0) ||
        ((pHeroThing->m_Flags34 & 0x10000000) == 0))
    {
        return;
    }

    ETCInterfaceType followedInterfaceType = static_cast<ETCInterfaceType>(0xBC);
    CKeyPair<ETCInterfaceType, CTCBase*>* pFollowedEntry =
        pHeroThing->m_InterfaceMap44.LowerBound(&followedInterfaceType);

    if ((pFollowedEntry == pHeroThing->m_InterfaceMap44.m_pEnd) ||
        (static_cast<ETCInterfaceType>(0xBC) < pFollowedEntry->m_Key))
    {
        pFollowedEntry = pHeroThing->m_InterfaceMap44.m_pEnd;
    }

    auto* const pFollowed = reinterpret_cast<CTCFollowed*>(pFollowedEntry->m_Value);
    if (!pFollowed->IsAIFollower(reinterpret_cast<CThingAICreature*>(pResolvedEntityThing)))
    {
        return;
    }

    pFollowed->RemoveFollower(reinterpret_cast<CThing*>(pResolvedEntityThing));
    RawThingCall006D5AB0(reinterpret_cast<CThing*>(pResolvedEntityThing));

    auto* const pFollowerThing = reinterpret_cast<FollowerThingOverlay*>(pResolvedEntityThing);
    if ((pFollowerThing->m_Flags28 & 0x04) != 0)
    {
        ETCInterfaceType entityEventsInterfaceType = static_cast<ETCInterfaceType>(0x42);
        CKeyPair<ETCInterfaceType, CTCBase*>* pEntityEventsEntry =
            pFollowerThing->m_InterfaceMap44.LowerBound(&entityEventsInterfaceType);

        if ((pEntityEventsEntry == pFollowerThing->m_InterfaceMap44.m_pEnd) ||
            (static_cast<ETCInterfaceType>(0x42) < pEntityEventsEntry->m_Key))
        {
            pEntityEventsEntry = pFollowerThing->m_InterfaceMap44.m_pEnd;
        }

        auto* const pEntityEventsInterface =
            reinterpret_cast<EntityEventsInterfaceOverlay*>(pEntityEventsEntry->m_Value);

        std::int32_t eventType = 3;
        RemoveIfEventType(&pEntityEventsInterface->m_EventList0C, &eventType);
    }
}
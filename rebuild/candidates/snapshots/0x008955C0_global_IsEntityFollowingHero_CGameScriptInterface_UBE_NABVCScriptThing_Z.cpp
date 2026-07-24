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
};

class CGameScriptInterface
{
public:
    bool __thiscall IsEntityFollowingHero(const CScriptThing& entity) const;
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

using Helper00449970Fn = void* (__thiscall*)(void*);
using Helper00487DC0Fn = HeroThingOverlay* (__thiscall*)(void*);

inline constexpr auto Helper00449970 =
    reinterpret_cast<Helper00449970Fn>(0x00449970);
inline constexpr auto Helper00487DC0 =
    reinterpret_cast<Helper00487DC0Fn>(0x00487DC0);
}

bool __thiscall CGameScriptInterface::IsEntityFollowingHero(const CScriptThing& entity) const
{
    const auto* const entityOverlay = reinterpret_cast<const CScriptThingOverlay*>(&entity);
    if (!entityOverlay->m_pVTable->IsValidThing(&entity))
    {
        return false;
    }

    auto* const pFollower =
        reinterpret_cast<CThingAICreature*>(entityOverlay->m_pVTable->GetThing(&entity));

    const auto* const self = reinterpret_cast<const CGameScriptInterfaceSelfOverlay*>(this);
    auto* const pHeroThing = Helper00487DC0(Helper00449970(self->m_Field14));

    if ((pHeroThing != nullptr) &&
        ((pHeroThing->m_Flags91 & 0x01) == 0) &&
        ((pHeroThing->m_Flags34 & 0x10000000) != 0))
    {
        ETCInterfaceType followedInterfaceType = static_cast<ETCInterfaceType>(0xBC);
        CKeyPair<ETCInterfaceType, CTCBase*>* pFollowedEntry =
            pHeroThing->m_InterfaceMap44.LowerBound(&followedInterfaceType);

        if ((pFollowedEntry == pHeroThing->m_InterfaceMap44.m_pEnd) ||
            (static_cast<ETCInterfaceType>(0xBC) < pFollowedEntry->m_Key))
        {
            pFollowedEntry = pHeroThing->m_InterfaceMap44.m_pEnd;
        }

        auto* const pFollowed = reinterpret_cast<CTCFollowed*>(pFollowedEntry->m_Value);
        if (pFollowed != nullptr)
        {
            return pFollowed->IsAIFollower(pFollower);
        }
    }

    return false;
}
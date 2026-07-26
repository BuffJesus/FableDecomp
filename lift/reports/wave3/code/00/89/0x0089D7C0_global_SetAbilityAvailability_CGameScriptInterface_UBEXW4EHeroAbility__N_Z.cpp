#include <cstddef>
#include <cstdint>

class CThingPlayerCreature;
class CTCBase;

enum EHeroAbility : std::int32_t;
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
    void __thiscall SetAbilityAvailability(EHeroAbility ability, bool enabled) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_SetAbilityAvailability_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};

static_assert(
    offsetof(CGameScriptInterface_SetAbilityAvailability_SelfOverlay, m_Field14) == 0x14);

struct CThingPlayerCreature_SetAbilityAvailability_Overlay
{
    std::byte m_Pad00[0x34];
    std::int8_t m_Field34; // 0x34
    std::byte m_Pad35[0x44 - 0x35];
    TCInterfaceMap m_InterfaceMap44; // 0x44
    std::byte m_Pad4C[0x91 - 0x4C];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(
    offsetof(CThingPlayerCreature_SetAbilityAvailability_Overlay, m_Field34) == 0x34);
static_assert(
    offsetof(CThingPlayerCreature_SetAbilityAvailability_Overlay, m_InterfaceMap44) == 0x44);
static_assert(
    offsetof(CThingPlayerCreature_SetAbilityAvailability_Overlay, m_Flags91) == 0x91);

struct CTCBase_SetAbilityAvailability_Overlay
{
    std::byte m_Pad00[0x1C];
    std::int32_t m_Field1C; // 0x1C
};

static_assert(
    offsetof(CTCBase_SetAbilityAvailability_Overlay, m_Field1C) == 0x1C);

using Helper00449970Fn = void* (__thiscall*)(void*);
using Helper00487DC0Fn = CThingPlayerCreature* (__thiscall*)(void*);

inline constexpr auto Helper00449970 =
    reinterpret_cast<Helper00449970Fn>(0x00449970);
inline constexpr auto Helper00487DC0 =
    reinterpret_cast<Helper00487DC0Fn>(0x00487DC0);

constexpr ETCInterfaceType kInterfaceTypeA7 = static_cast<ETCInterfaceType>(0xA7);
}

void __thiscall CGameScriptInterface::SetAbilityAvailability(
    EHeroAbility ability,
    bool enabled) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_SetAbilityAvailability_SelfOverlay*>(this);
    auto* const playerCreature = Helper00487DC0(Helper00449970(self->m_Field14));

    if ((playerCreature != nullptr) &&
        ((reinterpret_cast<const CThingPlayerCreature_SetAbilityAvailability_Overlay*>(playerCreature)
              ->m_Flags91 &
          0x01) == 0) &&
        (reinterpret_cast<const CThingPlayerCreature_SetAbilityAvailability_Overlay*>(playerCreature)
             ->m_Field34 < 0))
    {
        auto* const playerCreatureOverlay =
            reinterpret_cast<CThingPlayerCreature_SetAbilityAvailability_Overlay*>(playerCreature);

        CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
            playerCreatureOverlay->m_InterfaceMap44.LowerBound(&kInterfaceTypeA7);

        if ((pEntry == playerCreatureOverlay->m_InterfaceMap44.m_pEnd) ||
            (kInterfaceTypeA7 < pEntry->m_Key))
        {
            pEntry = playerCreatureOverlay->m_InterfaceMap44.m_pEnd;
        }

        const auto* const pInterface =
            reinterpret_cast<const CTCBase_SetAbilityAvailability_Overlay*>(pEntry->m_Value);

        *reinterpret_cast<bool*>(
            static_cast<std::uintptr_t>(static_cast<std::int32_t>(ability)) +
            static_cast<std::uintptr_t>(pInterface->m_Field1C)) = enabled;
    }
}
#include <cstddef>
#include <cstdint>

class CTCBase;
class CTCHeroExperience;

enum ETCInterfaceType : std::int32_t;

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
    void __thiscall GiveHeroExperience(long experience) const;
};

class CTCHeroExperience
{
public:
    void __thiscall AddExperience(long experience);
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_GiveHeroExperience_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};

static_assert(
    offsetof(CGameScriptInterface_GiveHeroExperience_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_GiveHeroExperience_TargetOverlay
{
    std::byte m_Pad00[0x2C];
    std::uint32_t m_Flags2C; // 0x2C
    std::byte m_Pad30[0x14];
    TCInterfaceMap m_TCInterfaceMap; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(
    offsetof(CGameScriptInterface_GiveHeroExperience_TargetOverlay, m_Flags2C) == 0x2C);
static_assert(
    offsetof(CGameScriptInterface_GiveHeroExperience_TargetOverlay, m_TCInterfaceMap) == 0x44);
static_assert(
    offsetof(CGameScriptInterface_GiveHeroExperience_TargetOverlay, m_Flags91) == 0x91);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC0 =
    CGameScriptInterface_GiveHeroExperience_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);
} // namespace

void __thiscall CGameScriptInterface::GiveHeroExperience(long experience) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_GiveHeroExperience_SelfOverlay*>(this);
    auto* const target = CALL_00487DC0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags2C & 0x00000100) != 0))
    {
        std::int32_t interfaceTypeValue = 0x68;

        auto* pEntry = target->m_TCInterfaceMap.LowerBound(
            reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue));

        if ((pEntry == target->m_TCInterfaceMap.m_pEnd) || (0x68 < pEntry->m_Key))
        {
            pEntry = target->m_TCInterfaceMap.m_pEnd;
        }

        reinterpret_cast<CTCHeroExperience*>(pEntry->m_Value)->AddExperience(experience);
    }
}
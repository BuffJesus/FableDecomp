#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;
class CTCHero;

enum ETCInterfaceType : std::int32_t;
enum ETutorialCategory : std::int32_t;

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

class CTCHero
{
public:
    bool __thiscall GiveTutorialForCategory(
        ETutorialCategory category,
        bool force_display);
};

class CGameScriptInterface
{
public:
    bool __thiscall DisplayTutorial(ETutorialCategory category) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_DisplayTutorial_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};

static_assert(
    offsetof(CGameScriptInterface_DisplayTutorial_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_DisplayTutorial_TargetOverlay
{
    std::byte m_Pad00[0x24];
    std::uint32_t m_Flags24; // 0x24
    std::byte m_Pad28[0x18];
    TCInterfaceMap m_TCInterfaceMap; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(
    offsetof(CGameScriptInterface_DisplayTutorial_TargetOverlay, m_Flags24) == 0x24);
static_assert(
    offsetof(CGameScriptInterface_DisplayTutorial_TargetOverlay, m_TCInterfaceMap) == 0x44);
static_assert(
    offsetof(CGameScriptInterface_DisplayTutorial_TargetOverlay, m_Flags91) == 0x91);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC0 =
    CGameScriptInterface_DisplayTutorial_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);
}

bool __thiscall CGameScriptInterface::DisplayTutorial(
    ETutorialCategory category) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_DisplayTutorial_SelfOverlay*>(this);
    auto* const target = CALL_00487DC0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags24 & 0x00000200) != 0))
    {
        std::int32_t interfaceTypeValue = 0x29;
        auto* pEntry = target->m_TCInterfaceMap.LowerBound(
            reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue));

        if ((pEntry == target->m_TCInterfaceMap.m_pEnd) || (0x29 < pEntry->m_Key))
        {
            pEntry = target->m_TCInterfaceMap.m_pEnd;
        }

        if (pEntry->m_Value != nullptr)
        {
            return reinterpret_cast<CTCHero*>(pEntry->m_Value)
                ->GiveTutorialForCategory(category, false);
        }
    }

    return false;
}
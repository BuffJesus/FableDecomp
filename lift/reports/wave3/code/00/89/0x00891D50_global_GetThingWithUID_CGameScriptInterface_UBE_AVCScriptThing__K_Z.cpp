#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CStopWatch;

namespace NHeroInformationScreens
{
class CBase
{
public:
    __thiscall CBase();
};
}

namespace NScript
{
class CIsThingAlive;

template <typename TLeft, typename TRight>
class CPredicate_And;

using GetThingWithUIDPredicate2 = CPredicate_And<CIsThingAlive, CIsThingAlive>;
using GetThingWithUIDPredicate3 = CPredicate_And<GetThingWithUIDPredicate2, CIsThingAlive>;
using GetThingWithUIDPredicate4 = CPredicate_And<GetThingWithUIDPredicate3, CIsThingAlive>;
using GetThingWithUIDPredicate5 = CPredicate_And<GetThingWithUIDPredicate4, CIsThingAlive>;

template <typename TPredicate, typename TResult>
void __fastcall GFPredicateAnd(
    TPredicate* pThing,
    CIsThingAlive* pPredicate,
    TResult* pResult);
}

class CStopWatch
{
public:
    std::uint64_t __thiscall GetTicks() const;
};

class CGameScriptInterface
{
public:
    CScriptThing* __thiscall GetThingWithUID(
        CScriptThing* pResult,
        std::uint64_t uid) const;
};

namespace
{
struct CGameScriptInterface_GetThingWithUID_SelfOverlay
{
    std::byte m_Pad00[0x04];
    void* m_Field04; // 0x04
};
static_assert(offsetof(CGameScriptInterface_GetThingWithUID_SelfOverlay, m_Field04) == 0x04);

struct CGameScriptInterface_GetThingWithUID_DispatchVftableOverlay
{
    std::byte m_Pad00[0x0C];
    CStopWatch* (__thiscall* m_Func0C)(
        void* pThis,
        std::uint32_t uidLow,
        std::uint32_t uidHigh); // 0x0C
};
static_assert(
    offsetof(CGameScriptInterface_GetThingWithUID_DispatchVftableOverlay, m_Func0C) == 0x0C);

struct CGameScriptInterface_GetThingWithUID_DispatchOverlay
{
    const CGameScriptInterface_GetThingWithUID_DispatchVftableOverlay* m_pVftable; // 0x00
};
static_assert(
    offsetof(CGameScriptInterface_GetThingWithUID_DispatchOverlay, m_pVftable) == 0x00);

struct CScriptThingOverlay
{
    void* m_pVftable;        // 0x00
    std::uint32_t m_Field04; // 0x04
    std::uint32_t m_Field08; // 0x08
};
static_assert(offsetof(CScriptThingOverlay, m_Field04) == 0x04);
static_assert(offsetof(CScriptThingOverlay, m_Field08) == 0x08);
}

CScriptThing* __thiscall CGameScriptInterface::GetThingWithUID(
    CScriptThing* pResult,
    const std::uint64_t uid) const
{
    const auto* const pThisOverlay =
        reinterpret_cast<const CGameScriptInterface_GetThingWithUID_SelfOverlay*>(this);

    CStopWatch* const pStopWatch =
        reinterpret_cast<const CGameScriptInterface_GetThingWithUID_DispatchOverlay*>(
            pThisOverlay->m_Field04)
            ->m_pVftable->m_Func0C(
                pThisOverlay->m_Field04,
                static_cast<std::uint32_t>(uid),
                static_cast<std::uint32_t>(uid >> 32));

    const std::uint64_t ticks = pStopWatch->GetTicks();
    if (static_cast<std::uint32_t>(ticks) != 0)
    {
        NScript::GFPredicateAnd<NScript::GetThingWithUIDPredicate4, CScriptThing>(
            reinterpret_cast<NScript::GetThingWithUIDPredicate4*>(
                static_cast<std::uint32_t>(ticks)),
            reinterpret_cast<NScript::CIsThingAlive*>(
                static_cast<std::uint32_t>(ticks >> 32)),
            pResult);
        return pResult;
    }

    NHeroInformationScreens::CBase::CBase(
        reinterpret_cast<NHeroInformationScreens::CBase*>(pResult));

    auto* const pResultOverlay = reinterpret_cast<CScriptThingOverlay*>(pResult);
    pResultOverlay->m_pVftable = reinterpret_cast<void*>(0x01238C8C);
    pResultOverlay->m_Field04 = 0;
    pResultOverlay->m_Field08 = 0;
    return pResult;
}
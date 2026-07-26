#include <cstddef>
#include <cstdint>
#include <utility>

class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CThing;

enum ETCInterfaceType : std::int32_t;
enum class EHeroMorphType : std::int32_t;

class CParticleMorphs
{
public:
    class CEntry;
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
    CScriptThing* __thiscall GetHeroTargetedThing(CScriptThing* pResult) const;
};

namespace NScript
{
class CIsThingAlive;

template <typename TLeft, typename TRight>
class CPredicate_And;

using GetHeroTargetedThingPredicate2 = CPredicate_And<CIsThingAlive, CIsThingAlive>;
using GetHeroTargetedThingPredicate3 = CPredicate_And<GetHeroTargetedThingPredicate2, CIsThingAlive>;
using GetHeroTargetedThingPredicate4 = CPredicate_And<GetHeroTargetedThingPredicate3, CIsThingAlive>;
using GetHeroTargetedThingPredicate5 = CPredicate_And<GetHeroTargetedThingPredicate4, CIsThingAlive>;

CScriptThing* __fastcall operator&&(GetHeroTargetedThingPredicate5* pLocalPredicate, CIsThingAlive* pRhs);
CScriptThing* __fastcall GFPredicateAnd(
    GetHeroTargetedThingPredicate4* pPredicate,
    CIsThingAlive* pRhs,
    GetHeroTargetedThingPredicate5* pLocalPredicate);
} // namespace NScript

namespace
{
using HeroMorphPair = std::pair<EHeroMorphType, CParticleMorphs::CEntry>;
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterfaceOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};

static_assert(offsetof(CGameScriptInterfaceOverlay, m_Field14) == 0x14);

struct HeroTargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint32_t m_Flags20; // 0x20
    std::byte m_Pad24[0x20];
    TCInterfaceMap m_InterfaceMap44; // 0x44
};

static_assert(offsetof(HeroTargetOverlay, m_Flags20) == 0x20);
static_assert(offsetof(HeroTargetOverlay, m_InterfaceMap44) == 0x44);

struct DispatchVftableOverlay
{
    std::byte m_Pad00[0x84];
    std::uint64_t(__thiscall* m_Func84)(void* pThis); // 0x84
};

static_assert(offsetof(DispatchVftableOverlay, m_Func84) == 0x84);

struct DispatchObjectOverlay
{
    const DispatchVftableOverlay* m_pVftable; // 0x00
};

static_assert(offsetof(DispatchObjectOverlay, m_pVftable) == 0x00);

struct PredicateSeed
{
    HeroTargetOverlay* m_TargetEax;
    NScript::CIsThingAlive* m_PredicateEdx;
};

using Catch_0040D959Fn = std::uint64_t(__thiscall*)(void*);
constexpr std::uintptr_t kConsValAddress = 0x00488DC2;
auto* const Catch_0040D959 = reinterpret_cast<Catch_0040D959Fn>(0x0040D959);

#if defined(_M_IX86)
[[nodiscard]] const HeroMorphPair* ReadIncomingEsi()
{
    const HeroMorphPair* value;
    __asm
    {
        mov value, esi
    }
    return value;
}

[[nodiscard]] PredicateSeed CallConsVal(
    const std::uint64_t sourceResult,
    const HeroMorphPair* const unaff_ESI)
{
    PredicateSeed result{};
    const std::uint32_t low32 = static_cast<std::uint32_t>(sourceResult);
    const std::uint32_t high32 = static_cast<std::uint32_t>(sourceResult >> 32);

    __asm
    {
        mov ecx, low32
        mov edx, high32
        mov esi, unaff_ESI
        mov eax, kConsValAddress
        call eax
        mov result.m_TargetEax, eax
        mov result.m_PredicateEdx, edx
    }

    return result;
}
#else
#error GetHeroTargetedThing reverse requires the original x86 register ABI.
#endif
} // namespace

CScriptThing* __thiscall CGameScriptInterface::GetHeroTargetedThing(CScriptThing* pResult) const
{
    const auto* const pThisOverlay = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);
    const PredicateSeed seed =
        CallConsVal(Catch_0040D959(pThisOverlay->m_Field14), ReadIncomingEsi());

    NScript::CIsThingAlive* pPredicate = seed.m_PredicateEdx;

    if (seed.m_TargetEax != nullptr)
    {
        void* pDispatchTarget = pResult;

        if ((seed.m_TargetEax->m_Flags20 & 0x100) != 0)
        {
            ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(8);
            CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
                seed.m_TargetEax->m_InterfaceMap44.LowerBound(&interfaceType);

            if ((pEntry == seed.m_TargetEax->m_InterfaceMap44.m_pEnd) || (8 < pEntry->m_Key))
            {
                pEntry = seed.m_TargetEax->m_InterfaceMap44.m_pEnd;
            }

            pDispatchTarget = pEntry->m_Value;
        }

        const std::uint64_t rawCallResult =
            reinterpret_cast<DispatchObjectOverlay*>(pDispatchTarget)->m_pVftable->m_Func84(
                pDispatchTarget);

        CThing* const pThing =
            reinterpret_cast<CThing*>(static_cast<std::uint32_t>(rawCallResult));
        pPredicate = reinterpret_cast<NScript::CIsThingAlive*>(
            static_cast<std::uint32_t>(rawCallResult >> 32));

        if (pThing != nullptr)
        {
            NScript::GFPredicateAnd(
                reinterpret_cast<NScript::GetHeroTargetedThingPredicate4*>(pThing),
                pPredicate,
                reinterpret_cast<NScript::GetHeroTargetedThingPredicate5*>(pResult));
            return pResult;
        }
    }

    NScript::operator&&(
        reinterpret_cast<NScript::GetHeroTargetedThingPredicate5*>(pResult),
        pPredicate);
    return pResult;
}
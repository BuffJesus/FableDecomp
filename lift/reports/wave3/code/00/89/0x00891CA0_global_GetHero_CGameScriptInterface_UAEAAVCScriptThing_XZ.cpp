#include <cstddef>
#include <cstdint>
#include <new>
#include <utility>

class CScriptThing
{
public:
    CScriptThing& operator=(const CScriptThing& other);
};

class CGameScriptInterface
{
public:
    CScriptThing& GetHero();
};

enum class EHeroMorphType : std::int32_t;

class CParticleMorphs
{
public:
    class CEntry;
};

namespace NScript
{
class CIsThingAlive;

template <typename TLeft, typename TRight>
class CPredicate_And;

using GetHeroPredicate3 =
    CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>, CIsThingAlive>;
using GetHeroPredicate4 = CPredicate_And<GetHeroPredicate3, CIsThingAlive>;
using GetHeroPredicate5 = CPredicate_And<GetHeroPredicate4, CIsThingAlive>;

CScriptThing* __fastcall operator&&(GetHeroPredicate5* predicate, CIsThingAlive* rhs);
CScriptThing* __fastcall GFPredicateAnd(GetHeroPredicate4* predicate, CIsThingAlive* rhs, GetHeroPredicate5* localPredicate);
}

namespace NHeroInformationScreens
{
class CBase
{
public:
    CBase();
};
}

namespace
{
using HeroMorphPair = std::pair<EHeroMorphType, CParticleMorphs::CEntry>;

struct CGameScriptInterface_GetHero_SelfOverlay
{
    std::byte pad_0x00[0x14];
    void* field_0x14;
    std::byte pad_0x18[0x18];
    CScriptThing result_0x30;
};

static_assert(offsetof(CGameScriptInterface_GetHero_SelfOverlay, field_0x14) == 0x14);
static_assert(offsetof(CGameScriptInterface_GetHero_SelfOverlay, result_0x30) == 0x30);

using SharedCleanupFn = void(__thiscall*)(void*);

struct GetHeroSharedCleanupOverlay
{
    std::int32_t refCount_0x00;
    SharedCleanupFn destroy_0x04;
    void* destroyThis_0x08;
};

static_assert(offsetof(GetHeroSharedCleanupOverlay, refCount_0x00) == 0x00);
static_assert(offsetof(GetHeroSharedCleanupOverlay, destroy_0x04) == 0x04);
static_assert(offsetof(GetHeroSharedCleanupOverlay, destroyThis_0x08) == 0x08);

struct GetHeroLocalOverlay
{
    void* vfptr_0x00;
    std::uint32_t field_0x04;
    GetHeroSharedCleanupOverlay* shared_0x08;
};

static_assert(offsetof(GetHeroLocalOverlay, vfptr_0x00) == 0x00);
static_assert(offsetof(GetHeroLocalOverlay, field_0x04) == 0x04);
static_assert(offsetof(GetHeroLocalOverlay, shared_0x08) == 0x08);

struct PredicateFactoryResult
{
    NScript::GetHeroPredicate4* eax;
    NScript::CIsThingAlive* edx;
};

using Catch_0040D959Fn = std::uint64_t(__thiscall*)(void*);

constexpr std::uintptr_t kConsValAddress = 0x00488DC2;
constexpr auto kScalarDeletingDestructor_01238C8C = reinterpret_cast<void*>(0x01238C8C);

auto* const Catch_0040D959 = reinterpret_cast<Catch_0040D959Fn>(0x0040D959);

#if defined(_M_IX86)
[[nodiscard]] const HeroMorphPair* ReadIncomingEdi()
{
    const HeroMorphPair* value;
    __asm
    {
        mov value, edi
    }
    return value;
}

[[nodiscard]] PredicateFactoryResult CallConsVal(
    const std::uint64_t uVar2,
    const HeroMorphPair* const unaff_EDI)
{
    PredicateFactoryResult result;
    const std::uint32_t low32 = static_cast<std::uint32_t>(uVar2);
    const std::uint32_t high32 = static_cast<std::uint32_t>(uVar2 >> 32);

    __asm
    {
        mov ecx, low32
        mov edx, high32
        mov edi, unaff_EDI
        mov eax, kConsValAddress
        call eax
        mov result.eax, eax
        mov result.edx, edx
    }

    return result;
}
#else
#error GetHero reverse requires the original x86 register ABI.
#endif
}

CScriptThing& CGameScriptInterface::GetHero()
{
    auto* const self = reinterpret_cast<CGameScriptInterface_GetHero_SelfOverlay*>(this);
    auto* const result = &self->result_0x30;
    const HeroMorphPair* const unaff_EDI = ReadIncomingEdi();

    alignas(GetHeroLocalOverlay) std::byte localStorage[sizeof(GetHeroLocalOverlay)];
    auto* const local = reinterpret_cast<GetHeroLocalOverlay*>(localStorage);

    const std::uint64_t uVar2 = Catch_0040D959(self->field_0x14);
    const PredicateFactoryResult predicateResult = CallConsVal(uVar2, unaff_EDI);

    CScriptThing* assignedThing;
    if (predicateResult.eax == nullptr)
    {
        assignedThing = NScript::operator&&(
            reinterpret_cast<NScript::GetHeroPredicate5*>(local),
            predicateResult.edx);
        result->operator=(*assignedThing);
        local->vfptr_0x00 = kScalarDeletingDestructor_01238C8C;
        if (local->shared_0x08 == nullptr)
        {
            goto LAB_00891D34;
        }

        --local->shared_0x08->refCount_0x00;
        if (local->shared_0x08->refCount_0x00 != 0)
        {
            goto LAB_00891D34;
        }

        local->shared_0x08->destroy_0x04(local->shared_0x08->destroyThis_0x08);
    }
    else
    {
        assignedThing = NScript::GFPredicateAnd(
            predicateResult.eax,
            predicateResult.edx,
            reinterpret_cast<NScript::GetHeroPredicate5*>(local));
        result->operator=(*assignedThing);
        local->vfptr_0x00 = kScalarDeletingDestructor_01238C8C;
        if (local->shared_0x08 == nullptr)
        {
            goto LAB_00891D34;
        }

        --local->shared_0x08->refCount_0x00;
        if (local->shared_0x08->refCount_0x00 != 0)
        {
            goto LAB_00891D34;
        }

        local->shared_0x08->destroy_0x04(local->shared_0x08->destroyThis_0x08);
    }

    ::operator delete(local->shared_0x08);

LAB_00891D34:
    local->shared_0x08 = nullptr;
    local->field_0x04 = 0;
    ::new (static_cast<void*>(local)) NHeroInformationScreens::CBase();
    return *result;
}
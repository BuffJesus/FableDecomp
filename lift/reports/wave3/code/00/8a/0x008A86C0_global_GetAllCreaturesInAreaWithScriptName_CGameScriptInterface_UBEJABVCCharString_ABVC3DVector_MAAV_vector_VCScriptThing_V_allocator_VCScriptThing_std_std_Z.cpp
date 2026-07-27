struct CCharString_Overlay
{
    CBasicString<char>* pString;
};
static_assert(offsetof(CCharString_Overlay, pString) == 0x00);

struct CGameScriptInterface_Overlay
{
    std::byte pad_0000[0x04];
    void* pContext;
};
static_assert(offsetof(CGameScriptInterface_Overlay, pContext) == 0x04);

struct CGameScriptInterface_Context_Overlay
{
    std::byte pad_0000[0x20];
    CThingSearchTools* pThingSearchTools;
};
static_assert(offsetof(CGameScriptInterface_Context_Overlay, pThingSearchTools) == 0x20);

struct CScriptThing_Shared_Overlay
{
    int refCount;
    void (__thiscall* destroy)(CScriptThing_Shared_Overlay*);
};
static_assert(offsetof(CScriptThing_Shared_Overlay, refCount) == 0x00);
static_assert(offsetof(CScriptThing_Shared_Overlay, destroy) == 0x04);

struct CScriptThing_Overlay
{
    void* vfptr;
    int value;
    CScriptThing_Shared_Overlay* shared;
};
static_assert(offsetof(CScriptThing_Overlay, vfptr) == 0x00);
static_assert(offsetof(CScriptThing_Overlay, value) == 0x04);
static_assert(offsetof(CScriptThing_Overlay, shared) == 0x08);
static_assert(sizeof(CScriptThing_Overlay) == 0x0C);

struct CScriptThingVector_Overlay
{
    CScriptThing_Overlay* first;
    CScriptThing_Overlay* last;
    CScriptThing_Overlay* end;
};
static_assert(offsetof(CScriptThingVector_Overlay, first) == 0x00);
static_assert(offsetof(CScriptThingVector_Overlay, last) == 0x04);
static_assert(offsetof(CScriptThingVector_Overlay, end) == 0x08);

struct SearchResultBuffer_Overlay
{
    CThingCreatureBase** first;
    CThingCreatureBase** last;
    int* capacityEnd;
};
static_assert(offsetof(SearchResultBuffer_Overlay, first) == 0x00);
static_assert(offsetof(SearchResultBuffer_Overlay, last) == 0x04);
static_assert(offsetof(SearchResultBuffer_Overlay, capacityEnd) == 0x08);

union LocalSearchThingStorage
{
    SearchResultBuffer_Overlay search;
    CScriptThing_Overlay thing;
};
static_assert(sizeof(LocalSearchThingStorage) == 0x0C);

struct EHeroMorphType;
namespace CParticleMorphs
{
    struct CEntry;
}

using ParticleMorphPair = std::pair<EHeroMorphType, CParticleMorphs::CEntry>;

struct Param3Storage
{
    alignas(std::max(alignof(CCharString), alignof(std::allocator<ParticleMorphPair>)))
        std::byte bytes[(sizeof(CCharString) > sizeof(std::allocator<ParticleMorphPair>))
                            ? sizeof(CCharString)
                            : sizeof(std::allocator<ParticleMorphPair>)];
};

extern void* PTR__scalar_deleting_destructor__01238c8c;

extern void __thiscall std__vector___Reserve(std::vector<CScriptThing>* self, int count);
extern void std__vector_InsertRange(
    CScriptThing_Overlay* insertAt,
    int scriptThingAddress,
    std::allocator<ParticleMorphPair>* param_3,
    int count,
    int one);
extern void std___Cons_val__allocator_pair_EHeroMorphType_CParticleMorphs_CEntry(
    std::allocator<ParticleMorphPair>* param_3,
    ParticleMorphPair* extraout_EDX,
    ParticleMorphPair* unaff_EDI);

unsigned long CGameScriptInterface::GetAllCreaturesInAreaWithScriptName(
    const CCharString& scriptName,
    const C3DVector& position,
    float radius,
    std::vector<CScriptThing>& outCreatures) const
{
    const int x = __ftol2(position.x);
    const int y = __ftol2(position.y);
    int r = __ftol2(radius);

    int left = x - r;
    int top = y - r;
    int right = x + r;
    int bottom = y + r;

    C2DBoxI roughArea;
    roughArea.left = left;
    roughArea.top = top;
    roughArea.right = right;
    roughArea.bottom = bottom;

    LocalSearchThingStorage local{};
    const auto* const thisOverlay = reinterpret_cast<const CGameScriptInterface_Overlay*>(this);
    const auto* const contextOverlay =
        reinterpret_cast<const CGameScriptInterface_Context_Overlay*>(thisOverlay->pContext);
    const auto* const scriptNameOverlay = reinterpret_cast<const CCharString_Overlay*>(&scriptName);

    ParticleMorphPair* extraout_EDX;
    ParticleMorphPair* unaff_EDI;

    if (scriptNameOverlay->pString == nullptr)
    {
        int remaining = 1;
        bool equal = true;
        const char* lhs = "";
        const char* rhs = "";

        do
        {
            if (remaining == 0)
            {
                break;
            }

            remaining = remaining - 1;
            equal = (*lhs == *rhs);
            lhs = lhs + 1;
            rhs = rhs + 1;
        } while (equal);

        if (equal)
        {
            goto search_without_name;
        }
    }
    else
    {
        const bool isEmpty = CBasicString<char>::operator==(scriptNameOverlay->pString, "");
        if (isEmpty)
        {
            goto search_without_name;
        }
    }

    {
        Param3Storage param_3;

        CCharString::CCharString(
            reinterpret_cast<CCharString*>(&param_3),
            const_cast<CCharString*>(&scriptName));
        CThingSearchTools::GetPBestThingInRoughArea<
            CFilter_Bind2<CThingFilter_IsNot, CIsVeryCloseMotionlessCreature>,
            CThingCompare_Nearest,
            CThingCreatureBase>(
                contextOverlay->pThingSearchTools,
                &roughArea,
                reinterpret_cast<CFilter_Bind2<CThingFilter_IsNot, CIsVeryCloseMotionlessCreature>*>(&param_3),
                reinterpret_cast<CThingCompare_Nearest*>(&local.search),
                reinterpret_cast<CThingCreatureBase**>(unaff_EDI));
        std___Cons_val__allocator_pair_EHeroMorphType_CParticleMorphs_CEntry(
            reinterpret_cast<std::allocator<ParticleMorphPair>*>(&param_3),
            extraout_EDX,
            unaff_EDI);
    }

    goto have_matches;

search_without_name:
    CThingSearchTools::GetPBestThingInRoughArea<
        CThingFilter_IsPotentialCustomer,
        CThingCompare_Nearest,
        CThingCreatureBase>(
            contextOverlay->pThingSearchTools,
            &roughArea,
            reinterpret_cast<CThingFilter_IsPotentialCustomer*>(const_cast<C3DVector*>(&position)),
            reinterpret_cast<CThingCompare_Nearest*>(&local.search),
            reinterpret_cast<CThingCreatureBase**>(unaff_EDI));

have_matches:
    CThingCreatureBase** const matchEnd = local.search.last;
    CThingCreatureBase** const matchBegin = local.search.first;
    auto* const outVector = reinterpret_cast<CScriptThingVector_Overlay*>(&outCreatures);

    if (local.search.first == local.search.last)
    {
        if (local.search.first != nullptr)
        {
            free(local.search.first);
        }
        return 0;
    }

    std__vector___Reserve(
        &outCreatures,
        (static_cast<int>(reinterpret_cast<std::uintptr_t>(matchEnd)) -
         static_cast<int>(reinterpret_cast<std::uintptr_t>(matchBegin))) >>
            2);

    CThingCreatureBase** current = matchBegin;
    do
    {
        std::allocator<ParticleMorphPair>* param_3;
        const int scriptThingAddress =
            NScript::GFPredicateAnd<
                NScript::CPredicate_And<
                    NScript::CPredicate_And<
                        NScript::CPredicate_And<
                            NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
                            NScript::CIsThingAlive>,
                        NScript::CIsThingAlive>,
                    NScript::CIsThingAlive>>(
                        *current,
                        reinterpret_cast<NScript::CIsThingAlive*>(&local.thing));

        CScriptThing_Overlay* const appendAt = outVector->last;
        if (appendAt == outVector->end)
        {
            std__vector_InsertRange(appendAt, scriptThingAddress, &param_3, 1, 1);
        }
        else
        {
            if (appendAt != nullptr)
            {
                appendAt->vfptr = PTR__scalar_deleting_destructor__01238c8c;
                appendAt->value = *reinterpret_cast<int*>(scriptThingAddress + 0x04);
                appendAt->shared =
                    *reinterpret_cast<CScriptThing_Shared_Overlay**>(scriptThingAddress + 0x08);
                if (appendAt->shared != nullptr)
                {
                    appendAt->shared->refCount = appendAt->shared->refCount + 1;
                }
            }

            outVector->last = reinterpret_cast<CScriptThing_Overlay*>(
                reinterpret_cast<std::byte*>(outVector->last) + 0x0C);
        }

        local.thing.vfptr = PTR__scalar_deleting_destructor__01238c8c;
        if (local.thing.shared != nullptr)
        {
            local.thing.shared->refCount = local.thing.shared->refCount - 1;
            if (local.thing.shared->refCount == 0)
            {
                local.thing.shared->destroy(local.thing.shared);
                operator delete(local.thing.shared);
            }
        }

        local.thing.value = 0;
        local.thing.shared = nullptr;
        NHeroInformationScreens::CBase::CBase(
            reinterpret_cast<NHeroInformationScreens::CBase*>(&local.thing));

        current = current + 1;
    } while (current != matchEnd);

    const auto* const reloadedVector = reinterpret_cast<const CScriptThingVector_Overlay*>(&outCreatures);
    const int firstByte = static_cast<int>(reinterpret_cast<std::uintptr_t>(reloadedVector->first));
    const int lastByte = static_cast<int>(reinterpret_cast<std::uintptr_t>(reloadedVector->last));

    if (matchBegin != nullptr)
    {
        free(matchBegin);
    }

    return static_cast<unsigned long>((lastByte - firstByte) / 0x0C);
}
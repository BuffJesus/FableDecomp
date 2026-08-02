template <typename T>
struct CBasicString;

struct CCharString;
struct C3DVector;
struct C2DBoxI;
struct CThingSearchTools;
struct CThingCreatureBase;
struct CThingFilter_IsPotentialCustomer;
struct CThingCompare_Nearest;
template <typename TFilter, typename TPredicate>
struct CFilter_Bind2;
struct CThingFilter_IsNot;
struct CIsVeryCloseMotionlessCreature;
struct CIsThingAlive;
struct CScriptThing;

namespace NScript
{
template <typename TPredicate>
CScriptThing* GFPredicateAnd(void* thing, CIsThingAlive* outValue);
}

namespace NHeroInformationScreens
{
struct CBase;
}

struct CCharString_Overlay
{
    CBasicString<char>* value;
};
static_assert(offsetof(CCharString_Overlay, value) == 0x0);

struct C3DVector_XY_Overlay
{
    float x;
    float y;
};
static_assert(offsetof(C3DVector_XY_Overlay, x) == 0x0);
static_assert(offsetof(C3DVector_XY_Overlay, y) == 0x4);

struct C2DBoxI_Overlay
{
    int left;
    int top;
    int right;
    int bottom;
};
static_assert(offsetof(C2DBoxI_Overlay, left) == 0x0);
static_assert(offsetof(C2DBoxI_Overlay, top) == 0x4);
static_assert(offsetof(C2DBoxI_Overlay, right) == 0x8);
static_assert(offsetof(C2DBoxI_Overlay, bottom) == 0xC);

struct CGameScriptInterface_SearchToolsOwner_Overlay
{
    std::byte pad_0x00[0x20];
    CThingSearchTools* searchTools;
};
static_assert(offsetof(CGameScriptInterface_SearchToolsOwner_Overlay, searchTools) == 0x20);

struct CGameScriptInterface_Overlay
{
    std::byte pad_0x00[0x04];
    CGameScriptInterface_SearchToolsOwner_Overlay* owner_0x04;
};
static_assert(offsetof(CGameScriptInterface_Overlay, owner_0x04) == 0x4);

struct CScriptThing_Shared_Overlay
{
    int refCount;
    void (__thiscall* destroy)(void* self);
    void* destroyThis;
};
static_assert(offsetof(CScriptThing_Shared_Overlay, refCount) == 0x0);
static_assert(offsetof(CScriptThing_Shared_Overlay, destroy) == 0x4);
static_assert(offsetof(CScriptThing_Shared_Overlay, destroyThis) == 0x8);

struct CScriptThing_Overlay
{
    void* vfptr;
    int field_0x04;
    CScriptThing_Shared_Overlay* shared_0x08;
};
static_assert(offsetof(CScriptThing_Overlay, vfptr) == 0x0);
static_assert(offsetof(CScriptThing_Overlay, field_0x04) == 0x4);
static_assert(offsetof(CScriptThing_Overlay, shared_0x08) == 0x8);
static_assert(sizeof(CScriptThing_Overlay) == 0x0C);

struct CScriptThing_Vector_Overlay
{
    CScriptThing_Overlay* first;
    CScriptThing_Overlay* last;
    CScriptThing_Overlay* end;

    void Reserve(int count);
    void InsertRange(
        CScriptThing_Overlay* insertAt,
        CScriptThing_Overlay* sourceFromPredicate,
        void* localTemporaryBuffer,
        int count,
        int one);
};
static_assert(offsetof(CScriptThing_Vector_Overlay, first) == 0x0);
static_assert(offsetof(CScriptThing_Vector_Overlay, last) == 0x4);
static_assert(offsetof(CScriptThing_Vector_Overlay, end) == 0x8);

struct CThingCreatureBasePtr_Vector_Overlay
{
    CThingCreatureBase** first;
    CThingCreatureBase** last;
    CThingCreatureBase** end;
};
static_assert(offsetof(CThingCreatureBasePtr_Vector_Overlay, first) == 0x0);
static_assert(offsetof(CThingCreatureBasePtr_Vector_Overlay, last) == 0x4);
static_assert(offsetof(CThingCreatureBasePtr_Vector_Overlay, end) == 0x8);

struct CScriptThing_Local_Overlay
{
    void* vfptr;
    int field_0x04;
    CScriptThing_Shared_Overlay* shared_0x08;
};
static_assert(offsetof(CScriptThing_Local_Overlay, vfptr) == 0x0);
static_assert(offsetof(CScriptThing_Local_Overlay, field_0x04) == 0x4);
static_assert(offsetof(CScriptThing_Local_Overlay, shared_0x08) == 0x8);
static_assert(sizeof(CScriptThing_Local_Overlay) == 0x0C);

struct ScriptNameTemp_Overlay
{
    std::byte storage[4];
};
static_assert(sizeof(ScriptNameTemp_Overlay) == 0x4);

extern void* PTR__scalar_deleting_destructor__01238c8c;

extern void __cdecl free(void* memory);
extern void __cdecl operator_delete(void* memory);

extern void ConstructCCharStringTemp(void* storage, const CCharString* source);
extern void PostSearchTempHelper(void* storage);

long CGameScriptInterface::GetAllCreaturesInAreaWithScriptName(
    const CCharString& scriptName,
    const C3DVector& areaCentre,
    float radius,
    std::vector<CScriptThing>& outCreatures) const
{
    const auto& thisOverlay = reinterpret_cast<const CGameScriptInterface_Overlay&>(*this);
    const auto& scriptNameOverlay = reinterpret_cast<const CCharString_Overlay&>(scriptName);
    const auto& centreOverlay = reinterpret_cast<const C3DVector_XY_Overlay&>(areaCentre);
    auto& outCreaturesOverlay = reinterpret_cast<CScriptThing_Vector_Overlay&>(outCreatures);

    const int areaX = static_cast<int>(centreOverlay.x);
    const int areaY = static_cast<int>(centreOverlay.y);
    int areaRadius = static_cast<int>(radius);

    C2DBoxI_Overlay roughArea;
    roughArea.left = areaX - areaRadius;
    roughArea.top = areaY - areaRadius;
    roughArea.right = areaX + areaRadius;
    roughArea.bottom = areaY + areaRadius;

    CThingCreatureBasePtr_Vector_Overlay roughMatches{};
    CScriptThing_Local_Overlay temporaryThing{};
    ScriptNameTemp_Overlay scriptNameTemp{};

    if (scriptNameOverlay.value == nullptr)
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
            CThingSearchTools::GetPBestThingInRoughArea<
                CThingFilter_IsPotentialCustomer,
                CThingCompare_Nearest,
                CThingCreatureBase>(
                    thisOverlay.owner_0x04->searchTools,
                    reinterpret_cast<const C2DBoxI*>(&roughArea),
                    reinterpret_cast<const CThingFilter_IsPotentialCustomer*>(&areaCentre),
                    reinterpret_cast<const CThingCompare_Nearest*>(&roughMatches));
            goto have_matches;
        }
    }
    else
    {
        if (CBasicString<char>::operator==(*scriptNameOverlay.value, ""))
        {
            CThingSearchTools::GetPBestThingInRoughArea<
                CThingFilter_IsPotentialCustomer,
                CThingCompare_Nearest,
                CThingCreatureBase>(
                    thisOverlay.owner_0x04->searchTools,
                    reinterpret_cast<const C2DBoxI*>(&roughArea),
                    reinterpret_cast<const CThingFilter_IsPotentialCustomer*>(&areaCentre),
                    reinterpret_cast<const CThingCompare_Nearest*>(&roughMatches));
            goto have_matches;
        }
    }

    ConstructCCharStringTemp(&scriptNameTemp, &scriptName);
    CThingSearchTools::GetPBestThingInRoughArea<
        CFilter_Bind2<CThingFilter_IsNot, CIsVeryCloseMotionlessCreature>,
        CThingCompare_Nearest,
        CThingCreatureBase>(
            thisOverlay.owner_0x04->searchTools,
            reinterpret_cast<const C2DBoxI*>(&roughArea),
            reinterpret_cast<const CFilter_Bind2<CThingFilter_IsNot, CIsVeryCloseMotionlessCreature>*>(&scriptNameTemp),
            reinterpret_cast<const CThingCompare_Nearest*>(&roughMatches));
    PostSearchTempHelper(&scriptNameTemp);

have_matches:
    CThingCreatureBase** const matchEnd = roughMatches.last;
    CThingCreatureBase** matchIt = roughMatches.first;

    if (roughMatches.first == roughMatches.last)
    {
        if (roughMatches.first != nullptr)
        {
            free(roughMatches.first);
        }

        return 0;
    }

    outCreaturesOverlay.Reserve(
        static_cast<int>(
            (reinterpret_cast<std::uintptr_t>(roughMatches.last) -
             reinterpret_cast<std::uintptr_t>(roughMatches.first)) >> 2));

    do
    {
        CScriptThing_Overlay* const predicateResult =
            reinterpret_cast<CScriptThing_Overlay*>(
                NScript::GFPredicateAnd<
                    NScript::CPredicate_And<
                        NScript::CPredicate_And<
                            NScript::CPredicate_And<
                                NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
                                NScript::CIsThingAlive>,
                            NScript::CIsThingAlive>,
                        NScript::CIsThingAlive>>(
                            *matchIt,
                            reinterpret_cast<CIsThingAlive*>(&temporaryThing)));

        CScriptThing_Overlay* const outLast = outCreaturesOverlay.last;
        if (outLast == outCreaturesOverlay.end)
        {
            outCreaturesOverlay.InsertRange(
                outLast,
                predicateResult,
                &scriptNameTemp,
                1,
                1);
        }
        else
        {
            if (outLast != nullptr)
            {
                outLast->vfptr = PTR__scalar_deleting_destructor__01238c8c;
                outLast->field_0x04 = predicateResult->field_0x04;
                CScriptThing_Shared_Overlay* const shared = predicateResult->shared_0x08;
                outLast->shared_0x08 = shared;
                if (shared != nullptr)
                {
                    shared->refCount = shared->refCount + 1;
                }
            }

            outCreaturesOverlay.last = reinterpret_cast<CScriptThing_Overlay*>(
                reinterpret_cast<std::byte*>(outCreaturesOverlay.last) + 0x0C);
        }

        temporaryThing.vfptr = PTR__scalar_deleting_destructor__01238c8c;
        if (temporaryThing.shared_0x08 != nullptr)
        {
            temporaryThing.shared_0x08->refCount = temporaryThing.shared_0x08->refCount - 1;
            if (temporaryThing.shared_0x08->refCount == 0)
            {
                temporaryThing.shared_0x08->destroy(temporaryThing.shared_0x08->destroyThis);
                operator_delete(temporaryThing.shared_0x08);
            }
        }

        temporaryThing.field_0x04 = 0;
        temporaryThing.shared_0x08 = nullptr;
        NHeroInformationScreens::CBase::CBase(
            reinterpret_cast<NHeroInformationScreens::CBase*>(&temporaryThing));

        matchIt = matchIt + 1;
    } while (matchIt != matchEnd);

    const int outBegin = reinterpret_cast<int>(outCreaturesOverlay.first);
    const int outEnd = reinterpret_cast<int>(outCreaturesOverlay.last);

    if (roughMatches.first != nullptr)
    {
        free(roughMatches.first);
    }

    return (outEnd - outBegin) / 0x0C;
}
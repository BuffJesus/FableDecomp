struct C2DBoxI
{
    int left;
    int top;
    int right;
    int bottom;
};

class CThingSearchTools;
class CThingCreatureBase;
class CCharString;
class C3DVector;
class CScriptThing;

template <typename T>
class CBasicString;

namespace NHeroInformationScreens
{
class CBase;
}

namespace NScript
{
class CIsThingAlive;

template <typename TPredicate, typename TThing>
TThing* GFPredicateAnd(TThing* thing, CIsThingAlive* outValue);
}

extern "C" int __cdecl __ftol2(float);
extern void* PTR__scalar_deleting_destructor__01238c8c;

namespace
{
#pragma pack(push, 1)

struct CGameScriptInterface_OuterOverlay
{
    std::byte pad_00[0x20];
    CThingSearchTools* thingSearchTools;
};
static_assert(offsetof(CGameScriptInterface_OuterOverlay, thingSearchTools) == 0x20);

struct CGameScriptInterface_Overlay
{
    std::byte pad_00[0x04];
    CGameScriptInterface_OuterOverlay* outer;
};
static_assert(offsetof(CGameScriptInterface_Overlay, outer) == 0x04);

struct IntrusiveSharedOverlay
{
    int refCount;
    void (__thiscall* destroy)(IntrusiveSharedOverlay*);
};
static_assert(offsetof(IntrusiveSharedOverlay, destroy) == 0x04);

struct CScriptThingStackFields
{
    void* scalarDeletingDtor;
    int field_04;
    IntrusiveSharedOverlay* shared;
};
static_assert(sizeof(CScriptThingStackFields) == 0x0C);
static_assert(offsetof(CScriptThingStackFields, scalarDeletingDtor) == 0x00);
static_assert(offsetof(CScriptThingStackFields, field_04) == 0x04);
static_assert(offsetof(CScriptThingStackFields, shared) == 0x08);

struct RawThingBuffer
{
    CThingCreatureBase** first;
    CThingCreatureBase** last;
    CThingCreatureBase** capacity;
};
static_assert(sizeof(RawThingBuffer) == 0x0C);

union Local1COverlay
{
    RawThingBuffer roughSearchResults;
    CScriptThingStackFields scriptThingTemp;
};
static_assert(sizeof(Local1COverlay) == 0x0C);

struct ReusedParam3Param4Stack
{
    std::byte raw[0x0C];
};
static_assert(sizeof(ReusedParam3Param4Stack) == 0x0C);

struct StdVectorScriptThingOverlay
{
    CScriptThingStackFields* first;
    CScriptThingStackFields* last;
    CScriptThingStackFields* end;
};
static_assert(sizeof(StdVectorScriptThingOverlay) == 0x0C);

#pragma pack(pop)

template <typename TFilter, typename TCompare, typename TThing>
extern void __thiscall GetPBestThingInRoughArea(
    CThingSearchTools* self,
    const C2DBoxI* roughArea,
    TFilter* filter,
    TCompare* compare);

extern void __thiscall std___Cons_val(
    ReusedParam3Param4Stack* reusedParam3Stack,
    void* extraout_edx_unknown,
    void* unaff_edi_unknown);

extern void __thiscall std__vector___Reserve(StdVectorScriptThingOverlay* self, int count);

extern void __thiscall std__vector_InsertRange(
    StdVectorScriptThingOverlay* self,
    CScriptThingStackFields* insertAt,
    const CScriptThingStackFields* sourceThing,
    const ReusedParam3Param4Stack* reusedParam3Stack,
    int oneA,
    int oneB);

extern void __thiscall CBase_CBase(NHeroInformationScreens::CBase* self);

template <typename T>
inline T*& UnderlyingBasicStringPtr(const CCharString& value)
{
    return *reinterpret_cast<T**>(const_cast<CCharString*>(&value));
}

} // namespace

long __thiscall CGameScriptInterface::GetAllCreaturesInAreaWithScriptName(
    const CCharString& scriptName,
    const C3DVector& position,
    float radius,
    std::vector<CScriptThing>& outThings) const
{
    const int xInt = __ftol2(position.x);
    const int yInt = __ftol2(position.y);
    int radiusInt = __ftol2(radius);

    C2DBoxI roughArea;
    roughArea.left = xInt - radiusInt;
    roughArea.top = yInt - radiusInt;
    roughArea.right = xInt + radiusInt;
    roughArea.bottom = yInt + radiusInt;

    Local1COverlay local1c{};
    ReusedParam3Param4Stack reusedParam3Stack{};

    auto* const selfOverlay = reinterpret_cast<const CGameScriptInterface_Overlay*>(this);
    CThingSearchTools* const thingSearchTools = selfOverlay->outer->thingSearchTools;

    if (UnderlyingBasicStringPtr<CBasicString<char>>(scriptName) == nullptr)
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

            --remaining;
            equal = (*lhs == *rhs);
            ++lhs;
            ++rhs;
        } while (equal);

        if (equal)
        {
            GetPBestThingInRoughArea<
                CThingFilter_IsPotentialCustomer,
                CThingCompare_Nearest,
                CThingCreatureBase>(
                thingSearchTools,
                &roughArea,
                reinterpret_cast<CThingFilter_IsPotentialCustomer*>(const_cast<C3DVector*>(&position)),
                reinterpret_cast<CThingCompare_Nearest*>(&local1c));
            goto after_search;
        }
    }
    else if (CBasicString<char>::operator==(UnderlyingBasicStringPtr<CBasicString<char>>(scriptName), ""))
    {
        GetPBestThingInRoughArea<
            CThingFilter_IsPotentialCustomer,
            CThingCompare_Nearest,
            CThingCreatureBase>(
            thingSearchTools,
            &roughArea,
            reinterpret_cast<CThingFilter_IsPotentialCustomer*>(const_cast<C3DVector*>(&position)),
            reinterpret_cast<CThingCompare_Nearest*>(&local1c));
        goto after_search;
    }

    CCharString::CCharString(reinterpret_cast<CCharString*>(&reusedParam3Stack), &scriptName);
    GetPBestThingInRoughArea<
        CFilter_Bind2<CThingFilter_IsNot, CIsVeryCloseMotionlessCreature>,
        CThingCompare_Nearest,
        CThingCreatureBase>(
        thingSearchTools,
        &roughArea,
        reinterpret_cast<CFilter_Bind2<CThingFilter_IsNot, CIsVeryCloseMotionlessCreature>*>(&reusedParam3Stack),
        reinterpret_cast<CThingCompare_Nearest*>(&local1c));
    std___Cons_val(
        &reusedParam3Stack,
        nullptr,   // extraout_EDX register-carried operand is present in Ghidra but not proven from source context
        nullptr);  // unaff_EDI register-carried operand is present in Ghidra but not proven from source context

after_search:
    CThingCreatureBase** const memory = local1c.roughSearchResults.first;
    CThingCreatureBase** const finish = local1c.roughSearchResults.last;
    auto* const outOverlay = reinterpret_cast<StdVectorScriptThingOverlay*>(&outThings);

    if (memory == finish)
    {
        if (memory != nullptr)
        {
            free(memory);
        }
        return 0;
    }

    std__vector___Reserve(outOverlay, static_cast<int>((reinterpret_cast<std::uintptr_t>(finish) - reinterpret_cast<std::uintptr_t>(memory)) >> 2));

    for (CThingCreatureBase** it = memory; it != finish; ++it)
    {
        auto* const producedThing = reinterpret_cast<CScriptThingStackFields*>(
            NScript::GFPredicateAnd<
                NScript::CPredicate_And<
                    NScript::CPredicate_And<
                        NScript::CPredicate_And<
                            NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
                            NScript::CIsThingAlive>,
                        NScript::CIsThingAlive>,
                    NScript::CIsThingAlive>,
                CThingCreatureBase>(
                *it,
                reinterpret_cast<NScript::CIsThingAlive*>(&local1c)));

        CScriptThingStackFields* const insertAt = outOverlay->last;
        if (insertAt == outOverlay->end)
        {
            std__vector_InsertRange(
                outOverlay,
                insertAt,
                producedThing,
                &reusedParam3Stack,
                1,
                1);
        }
        else
        {
            if (insertAt != nullptr)
            {
                insertAt->scalarDeletingDtor = PTR__scalar_deleting_destructor__01238c8c;
                insertAt->field_04 = producedThing->field_04;
                insertAt->shared = producedThing->shared;
                if (insertAt->shared != nullptr)
                {
                    ++insertAt->shared->refCount;
                }
            }
            outOverlay->last = reinterpret_cast<CScriptThingStackFields*>(reinterpret_cast<std::byte*>(outOverlay->last) + 0x0C);
        }

        local1c.scriptThingTemp.scalarDeletingDtor = PTR__scalar_deleting_destructor__01238c8c;
        if (local1c.scriptThingTemp.shared != nullptr)
        {
            --local1c.scriptThingTemp.shared->refCount;
            if (local1c.scriptThingTemp.shared->refCount == 0)
            {
                local1c.scriptThingTemp.shared->destroy(local1c.scriptThingTemp.shared);
                operator delete(local1c.scriptThingTemp.shared);
            }
        }
        local1c.scriptThingTemp.field_04 = 0;
        local1c.scriptThingTemp.shared = nullptr;
        CBase_CBase(reinterpret_cast<NHeroInformationScreens::CBase*>(&local1c));
    }

    const long result = static_cast<long>(
        (reinterpret_cast<std::uintptr_t>(outOverlay->last) - reinterpret_cast<std::uintptr_t>(outOverlay->first)) / 0x0C);

    if (memory != nullptr)
    {
        free(memory);
    }

    return result;
}
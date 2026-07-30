long CGameScriptInterface::GetAllCreaturesInAreaWithScriptName(
    const CCharString& scriptName,
    const C3DVector& areaCenter,
    float radius,
    std::vector<CScriptThing>& outThings) const
{
    struct CGameScriptInterface_Overlay
    {
        std::byte pad_00[0x04];
        void* owner;
    };
    static_assert(offsetof(CGameScriptInterface_Overlay, owner) == 0x04);

    struct OwnerOverlay
    {
        std::byte pad_00[0x20];
        CThingSearchTools* thingSearchTools;
    };
    static_assert(offsetof(OwnerOverlay, thingSearchTools) == 0x20);

    struct VectorOverlay
    {
        std::byte* first;
        std::byte* last;
        std::byte* end;
    };
    static_assert(offsetof(VectorOverlay, first) == 0x00);
    static_assert(offsetof(VectorOverlay, last) == 0x04);
    static_assert(offsetof(VectorOverlay, end) == 0x08);

    struct RefCountedOverlay
    {
        int refCount;
        std::uintptr_t releaseCallback;
    };
    static_assert(offsetof(RefCountedOverlay, refCount) == 0x00);
    static_assert(offsetof(RefCountedOverlay, releaseCallback) == 0x04);

    struct CScriptThingOverlay
    {
        void* vfptr;
        int payload;
        RefCountedOverlay* shared;
    };
    static_assert(offsetof(CScriptThingOverlay, vfptr) == 0x00);
    static_assert(offsetof(CScriptThingOverlay, payload) == 0x04);
    static_assert(offsetof(CScriptThingOverlay, shared) == 0x08);
    static_assert(sizeof(CScriptThingOverlay) == 0x0C);

    struct Param34Overlay
    {
        const C3DVector* areaCenterRef;
        float radiusValue;
    };
    static_assert(sizeof(Param34Overlay) == 0x08);

    struct ParticleMorphPair;

    extern void* PTR__scalar_deleting_destructor__01238c8c;
    extern void std___Cons_val_param3(void* param3, ParticleMorphPair* extraoutEDX, ParticleMorphPair* unaffEDI);

    const auto* const self = reinterpret_cast<const CGameScriptInterface_Overlay*>(this);
    const auto* const owner = reinterpret_cast<const OwnerOverlay*>(self->owner);
    auto* const outVector = reinterpret_cast<VectorOverlay*>(&outThings);

    const int x = __ftol2(areaCenter.x);
    const int y = __ftol2(areaCenter.y);
    int r = __ftol2(radius);

    int left = x - r;
    int top = y - r;
    int right = x + r;
    int bottom = y + r;

    void** local_1c = nullptr;
    void** local_18 = nullptr;
    int* local_14 = nullptr;

    auto* const param_3 = reinterpret_cast<Param34Overlay*>(
        static_cast<std::byte*>(_AddressOfReturnAddress()) + 0x08);

    if (*reinterpret_cast<CBasicString<char>* const*>(&scriptName) == nullptr)
    {
        int i = 1;
        bool equal = true;
        const char* lhs = "";
        const char* rhs = "";

        do
        {
            if (i == 0)
            {
                break;
            }

            i = i - 1;
            equal = (*lhs == *rhs);
            ++lhs;
            ++rhs;
        } while (equal);

        if (equal)
        {
            owner->thingSearchTools
                ->GetPBestThingInRoughArea<
                    CThingFilter_IsPotentialCustomer,
                    CThingCompare_Nearest,
                    CThingCreatureBase>(
                    reinterpret_cast<C2DBoxI*>(&left),
                    reinterpret_cast<CThingFilter_IsPotentialCustomer*>(param_3),
                    reinterpret_cast<CThingCompare_Nearest*>(&local_1c));
            goto build_output;
        }
    }
    else
    {
        if (CBasicString<char>::operator==(
                *reinterpret_cast<CBasicString<char>* const*>(&scriptName),
                ""))
        {
            owner->thingSearchTools
                ->GetPBestThingInRoughArea<
                    CThingFilter_IsPotentialCustomer,
                    CThingCompare_Nearest,
                    CThingCreatureBase>(
                    reinterpret_cast<C2DBoxI*>(&left),
                    reinterpret_cast<CThingFilter_IsPotentialCustomer*>(param_3),
                    reinterpret_cast<CThingCompare_Nearest*>(&local_1c));
            goto build_output;
        }
    }

    CCharString::CCharString(reinterpret_cast<CCharString*>(param_3), &scriptName);

    ParticleMorphPair* extraout_EDX;
    ParticleMorphPair* unaff_EDI;

    owner->thingSearchTools
        ->GetPBestThingInRoughArea<
            CFilter_Bind2<CThingFilter_IsNot, CIsVeryCloseMotionlessCreature>,
            CThingCompare_Nearest,
            CThingCreatureBase>(
            reinterpret_cast<C2DBoxI*>(&left),
            reinterpret_cast<CFilter_Bind2<CThingFilter_IsNot, CIsVeryCloseMotionlessCreature>*>(param_3),
            reinterpret_cast<CThingCompare_Nearest*>(&local_1c));

#if defined(_M_IX86)
    __asm
    {
        mov extraout_EDX, edx
        mov unaff_EDI, edi
    }
#else
    extraout_EDX = nullptr;
    unaff_EDI = nullptr;
#endif

    std___Cons_val_param3(param_3, extraout_EDX, unaff_EDI);

build_output:
    if (local_1c == local_18)
    {
        if (local_1c != nullptr)
        {
            free(local_1c);
        }
        return 0;
    }

    std__vector___Reserve((static_cast<int>(reinterpret_cast<std::uintptr_t>(local_18)) -
                           static_cast<int>(reinterpret_cast<std::uintptr_t>(local_1c))) >>
                          2);

    void** current = local_1c;
    void** end = local_18;

    CScriptThingOverlay temp{};
    temp.vfptr = &PTR__scalar_deleting_destructor__01238c8c;
    temp.payload = 0;
    temp.shared = nullptr;

    do
    {
        const int produced = NScript::GFPredicateAnd<
            NScript::CPredicate_And<
                NScript::CPredicate_And<
                    NScript::CPredicate_And<
                        NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
                        NScript::CIsThingAlive>,
                    NScript::CIsThingAlive>,
                NScript::CIsThingAlive>>(
            *current,
            reinterpret_cast<CIsThingAlive*>(&temp));

        std::byte* const rawEnd = outVector->last;
        if (rawEnd == outVector->end)
        {
            std__vector_InsertRange(rawEnd, produced, param_3, 1, 1);
        }
        else
        {
            auto* const dst = reinterpret_cast<CScriptThingOverlay*>(rawEnd);
            if (dst != nullptr)
            {
                dst->vfptr = &PTR__scalar_deleting_destructor__01238c8c;
                dst->payload = *reinterpret_cast<const int*>(produced + 0x04);
                dst->shared = *reinterpret_cast<RefCountedOverlay* const*>(produced + 0x08);
                if (dst->shared != nullptr)
                {
                    dst->shared->refCount = dst->shared->refCount + 1;
                }
            }
            outVector->last = rawEnd + 0x0C;
        }

        temp.vfptr = &PTR__scalar_deleting_destructor__01238c8c;
        if (temp.shared != nullptr)
        {
            temp.shared->refCount = temp.shared->refCount - 1;
            if (temp.shared->refCount == 0)
            {
#if defined(_M_IX86)
                RefCountedOverlay* const releaseObject = temp.shared;
                __asm
                {
                    mov ecx, releaseObject
                    call dword ptr [ecx + 4]
                }
#else
                reinterpret_cast<void(__thiscall*)(RefCountedOverlay*)>(
                    temp.shared->releaseCallback)(temp.shared);
#endif
                operator_delete(temp.shared);
            }
        }

        temp.payload = 0;
        temp.shared = nullptr;
        NHeroInformationScreens::CBase::CBase(
            reinterpret_cast<NHeroInformationScreens::CBase*>(&temp));

        ++current;
    } while (current != end);

    const int first = reinterpret_cast<int>(outVector->first);
    const int last = reinterpret_cast<int>(outVector->last);

    if (local_1c != nullptr)
    {
        free(local_1c);
    }

    return (last - first) / 0x0C;
}
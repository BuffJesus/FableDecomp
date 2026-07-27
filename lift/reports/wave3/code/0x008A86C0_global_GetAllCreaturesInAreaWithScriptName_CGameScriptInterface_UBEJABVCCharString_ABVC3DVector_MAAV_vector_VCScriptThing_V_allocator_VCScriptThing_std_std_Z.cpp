int CGameScriptInterface::GetAllCreaturesInAreaWithScriptName(
    const CCharString& scriptName,
    const C3DVector& position,
    float radius,
    std::vector<CScriptThing>& outThings) const
{
    struct CGameScriptInterface_Overlay
    {
        std::byte pad_00[0x04];
        void* field_04;
    };
    static_assert(offsetof(CGameScriptInterface_Overlay, field_04) == 0x04);

    struct Field04_Overlay
    {
        std::byte pad_00[0x20];
        CThingSearchTools* thingSearchTools;
    };
    static_assert(offsetof(Field04_Overlay, thingSearchTools) == 0x20);

    struct PointerBuffer
    {
        void** begin;
        void** end;
        void** capacity;
    };
    static_assert(sizeof(PointerBuffer) == 0x0C);

    struct RefCountedPayloadOverlay
    {
        int refCount;
        std::uintptr_t destroyFn;
    };
    static_assert(offsetof(RefCountedPayloadOverlay, refCount) == 0x00);
    static_assert(offsetof(RefCountedPayloadOverlay, destroyFn) == 0x04);

    struct ScriptThingStackOverlay
    {
        void* field_00;
        std::uint32_t field_04;
        RefCountedPayloadOverlay* field_08;
    };
    static_assert(sizeof(ScriptThingStackOverlay) == 0x0C);
    static_assert(offsetof(ScriptThingStackOverlay, field_00) == 0x00);
    static_assert(offsetof(ScriptThingStackOverlay, field_04) == 0x04);
    static_assert(offsetof(ScriptThingStackOverlay, field_08) == 0x08);

    union Param3Overlay
    {
        CCharString stringValue;
        ScriptThingStackOverlay thingValue;
        void* raw;
    };
    static_assert(sizeof(Param3Overlay) >= sizeof(ScriptThingStackOverlay));

    struct VectorOverlay
    {
        CScriptThing* first;
        CScriptThing* last;
        CScriptThing* end;
    };
    static_assert(sizeof(VectorOverlay) == 0x0C);

    using DestroyFn = void(__thiscall*)(RefCountedPayloadOverlay*);

    const int iVar5 = __ftol2(position.x);
    const int iVar6 = __ftol2(position.y);
    int local_4 = __ftol2(radius);

    int local_10 = iVar5 - local_4;
    int local_c = iVar6 - local_4;
    int local_8 = iVar5 + local_4;
    local_4 = iVar6 + local_4;

    void** local_1c = nullptr;
    void** local_18 = nullptr;
    RefCountedPayloadOverlay* local_14 = nullptr;

    auto* const searchTools =
        reinterpret_cast<Field04_Overlay*>(
            reinterpret_cast<const CGameScriptInterface_Overlay*>(this)->field_04)
            ->thingSearchTools;

    if (*reinterpret_cast<CBasicString<char>* const*>(&scriptName) == nullptr)
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
            searchTools->GetPBestThingInRoughArea<
                CThingFilter_IsPotentialCustomer,
                CThingCompare_Nearest,
                CThingCreatureBase>(
                *reinterpret_cast<C2DBoxI*>(&local_10),
                &position,
                *reinterpret_cast<PointerBuffer*>(&local_1c));
            goto process_results;
        }
    }
    else if (scriptName == "")
    {
        searchTools->GetPBestThingInRoughArea<
            CThingFilter_IsPotentialCustomer,
            CThingCompare_Nearest,
            CThingCreatureBase>(
            *reinterpret_cast<C2DBoxI*>(&local_10),
            &position,
            *reinterpret_cast<PointerBuffer*>(&local_1c));
        goto process_results;
    }

    {
        Param3Overlay param_3{};
        CCharString::CCharString(reinterpret_cast<CCharString*>(&param_3), &scriptName);
        searchTools->GetPBestThingInRoughArea<
            CFilter_Bind2<CThingFilter_IsNot, CIsVeryCloseMotionlessCreature>,
            CThingCompare_Nearest,
            CThingCreatureBase>(
            *reinterpret_cast<C2DBoxI*>(&local_10),
            reinterpret_cast<CFilter_Bind2<CThingFilter_IsNot, CIsVeryCloseMotionlessCreature>*>(&param_3),
            *reinterpret_cast<PointerBuffer*>(&local_1c));
        std::_Cons_val<
            std::allocator<std::pair<EHeroMorphType, CParticleMorphs::CEntry>>,
            std::pair<EHeroMorphType, CParticleMorphs::CEntry>,
            const std::pair<EHeroMorphType, CParticleMorphs::CEntry>&>(
            reinterpret_cast<std::allocator<std::pair<EHeroMorphType, CParticleMorphs::CEntry>>*>(&param_3));
    }

process_results:
    void** const ppuVar4 = local_18;
    void** const memory = local_1c;
    auto& outRaw = reinterpret_cast<VectorOverlay&>(outThings);

    if (local_1c == local_18)
    {
        if (local_1c != nullptr)
        {
            free(local_1c);
        }
        return 0;
    }

    outThings.reserve(static_cast<std::size_t>((reinterpret_cast<std::uintptr_t>(local_18) - reinterpret_cast<std::uintptr_t>(local_1c)) >> 2));

    void** ppuVar9 = memory;
    do
    {
        Param3Overlay param_3{};
        const int predicateResult = NScript::GFPredicateAnd<
            NScript::CPredicate_And<
                NScript::CPredicate_And<
                    NScript::CPredicate_And<
                        NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
                        NScript::CIsThingAlive>,
                    NScript::CIsThingAlive>,
                NScript::CIsThingAlive>>(
                    *ppuVar9,
                    reinterpret_cast<CIsThingAlive*>(&param_3.thingValue));

        auto* const puVar1 = reinterpret_cast<ScriptThingStackOverlay*>(outRaw.last);
        if (puVar1 == reinterpret_cast<ScriptThingStackOverlay*>(outRaw.end))
        {
            std__vector_InsertRange(puVar1, predicateResult, &param_3, 1, 1);
        }
        else
        {
            puVar1->field_00 = PTR__scalar_deleting_destructor__01238c8c;
            puVar1->field_04 = *reinterpret_cast<std::uint32_t*>(predicateResult + 4);
            auto* const piVar2 = *reinterpret_cast<RefCountedPayloadOverlay**>(predicateResult + 8);
            puVar1->field_08 = piVar2;
            if (piVar2 != nullptr)
            {
                ++piVar2->refCount;
            }
            outRaw.last = reinterpret_cast<CScriptThing*>(
                reinterpret_cast<std::byte*>(outRaw.last) + 0x0C);
        }

        param_3.thingValue.field_00 = PTR__scalar_deleting_destructor__01238c8c;
        if (param_3.thingValue.field_08 != nullptr)
        {
            --param_3.thingValue.field_08->refCount;
            if (param_3.thingValue.field_08->refCount == 0)
            {
                reinterpret_cast<DestroyFn>(param_3.thingValue.field_08->destroyFn)(
                    param_3.thingValue.field_08);
                ::operator delete(param_3.thingValue.field_08);
            }
        }
        param_3.thingValue.field_04 = 0;
        param_3.thingValue.field_08 = nullptr;
        NHeroInformationScreens::CBase::CBase(
            reinterpret_cast<NHeroInformationScreens::CBase*>(&param_3.thingValue));

        ++ppuVar9;
    } while (ppuVar9 != ppuVar4);

    const int first = reinterpret_cast<int>(outRaw.first);
    const int last = reinterpret_cast<int>(outRaw.last);

    if (memory != nullptr)
    {
        free(memory);
    }

    return (last - first) / 0x0C;
}
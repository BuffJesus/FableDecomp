int CGameScriptInterface::GetAllThingsWithDefNameByDistanceFrom(
    const CScriptThing& fromThing,
    const CCharString& defName,
    std::vector<CScriptThing>& outThings) const
{
    struct CGameScriptInterface_Layout
    {
        std::byte pad_00[0x04];
        void* searchOwner_04;
    };
    static_assert(offsetof(CGameScriptInterface_Layout, searchOwner_04) == 0x04);

    struct SearchOwner_Layout
    {
        std::byte pad_00[0x20];
        CThingSearchTools* thingSearchTools_20;
    };
    static_assert(offsetof(SearchOwner_Layout, thingSearchTools_20) == 0x20);

    struct RefCountBlock
    {
        int refCount_00;
        void (__thiscall* destroy_04)(RefCountBlock*);
    };
    static_assert(offsetof(RefCountBlock, refCount_00) == 0x00);
    static_assert(offsetof(RefCountBlock, destroy_04) == 0x04);

    struct CScriptThing_Overlay
    {
        void* field_00;
        void* field_04;
        RefCountBlock* field_08;
    };
    static_assert(sizeof(CScriptThing_Overlay) == 0x0C);
    static_assert(offsetof(CScriptThing_Overlay, field_00) == 0x00);
    static_assert(offsetof(CScriptThing_Overlay, field_04) == 0x04);
    static_assert(offsetof(CScriptThing_Overlay, field_08) == 0x08);

    template <typename T>
    struct VectorOverlay
    {
        T* first_00;
        T* last_04;
        T* end_08;
    };
    static_assert(sizeof(VectorOverlay<CThing*>) == 0x0C);
    static_assert(sizeof(VectorOverlay<CScriptThing>) == 0x0C);

    union LocalStorage
    {
        VectorOverlay<CThing*> foundThings;
        CScriptThing_Overlay tempThing;
    };
    static_assert(sizeof(LocalStorage) == 0x0C);

    const auto* const selfLayout = reinterpret_cast<const CGameScriptInterface_Layout*>(this);
    const auto* const searchOwner = reinterpret_cast<const SearchOwner_Layout*>(selfLayout->searchOwner_04);

    LocalStorage localStorage{};
    std::uintptr_t param3Slot = static_cast<std::uintptr_t>(
        CDefinitionManager::GetDefGlobalIndexFromName(
            reinterpret_cast<CDefinitionManager*>(GFGetPreMainMemoryUsed()),
            const_cast<CCharString*>(&defName)));

    CThingSearchTools::GetAllThings<
        CThing,
        std::vector<CThing*, std::allocator<CThing*>>,
        CFilter_Bind2<CThingFilter_InMap, std::unary_negate<CThingFilter_InArea>>
    >(
        searchOwner->thingSearchTools_20,
        reinterpret_cast<CFilter_Bind2<CThingFilter_InMap, std::unary_negate<CThingFilter_InArea>>*>(&param3Slot),
        reinterpret_cast<std::vector<CThing*, std::allocator<CThing*>>*>(&localStorage.foundThings));

    CThing** const foundBegin = localStorage.foundThings.first_00;
    CThing** const foundEnd = localStorage.foundThings.last_04;
    VectorOverlay<CScriptThing>* const outVector =
        reinterpret_cast<VectorOverlay<CScriptThing>*>(&outThings);

    if (foundBegin == foundEnd)
    {
        if (foundBegin != nullptr)
        {
            std::free(foundBegin);
        }
        return 0;
    }

    std__vector___Reserve(
        &outThings,
        (static_cast<int>(reinterpret_cast<std::uintptr_t>(foundEnd)) -
         static_cast<int>(reinterpret_cast<std::uintptr_t>(foundBegin))) >> 2);

    using SortSeedFn = unsigned int* (__thiscall*)(const CScriptThing*);
    const auto* const vftable = *reinterpret_cast<SortSeedFn const* const*>(&fromThing);
    unsigned int* const sortSeed = vftable[6](&fromThing);
    MakeHeap_AndSort(sortSeed[0], sortSeed[1], sortSeed[2]);

    CThing** current = foundBegin;
    do
    {
        const int predicateResult = NScript::GFPredicateAnd<
            NScript::CPredicate_And<
                NScript::CPredicate_And<
                    NScript::CPredicate_And<
                        NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
                        NScript::CIsThingAlive>,
                    NScript::CIsThingAlive>,
                NScript::CIsThingAlive>,
            NScript::CIsThingAlive>(
                *current,
                reinterpret_cast<NScript::CIsThingAlive*>(&localStorage.tempThing));

        CScriptThing* const insertAt = outVector->last_04;
        const auto* const predicateThing =
            reinterpret_cast<const CScriptThing_Overlay*>(predicateResult);

        if (insertAt == outVector->end_08)
        {
            std__vector_InsertRange(
                insertAt,
                predicateResult,
                &param3Slot,
                1,
                1);
        }
        else
        {
            if (insertAt != nullptr)
            {
                auto* const dst = reinterpret_cast<CScriptThing_Overlay*>(insertAt);
                dst->field_00 = &PTR__scalar_deleting_destructor__01238c8c;
                dst->field_04 = predicateThing->field_04;
                dst->field_08 = predicateThing->field_08;
                if (dst->field_08 != nullptr)
                {
                    ++dst->field_08->refCount_00;
                }
            }
            outVector->last_04 = reinterpret_cast<CScriptThing*>(
                reinterpret_cast<std::byte*>(outVector->last_04) + 0x0C);
        }

        localStorage.tempThing.field_00 = &PTR__scalar_deleting_destructor__01238c8c;
        if (localStorage.tempThing.field_08 != nullptr)
        {
            --localStorage.tempThing.field_08->refCount_00;
            if (localStorage.tempThing.field_08->refCount_00 == 0)
            {
                localStorage.tempThing.field_08->destroy_04(localStorage.tempThing.field_08);
                operator_delete(localStorage.tempThing.field_08);
            }
        }

        localStorage.tempThing.field_04 = nullptr;
        localStorage.tempThing.field_08 = nullptr;
        NHeroInformationScreens::CBase::CBase(
            reinterpret_cast<NHeroInformationScreens::CBase*>(&localStorage.tempThing));

        ++current;
    } while (current != foundEnd);

    const int outBegin = static_cast<int>(reinterpret_cast<std::uintptr_t>(outVector->first_00));
    const int outLast = static_cast<int>(reinterpret_cast<std::uintptr_t>(outVector->last_04));

    if (foundBegin != nullptr)
    {
        std::free(foundBegin);
    }

    return (outLast - outBegin) / 0x0C;
}
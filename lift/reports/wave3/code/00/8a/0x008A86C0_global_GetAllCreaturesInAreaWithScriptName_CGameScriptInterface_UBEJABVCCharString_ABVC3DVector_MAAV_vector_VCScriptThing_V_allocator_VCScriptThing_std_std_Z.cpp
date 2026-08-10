namespace
{
    struct C2DBoxI
    {
        int32_t left;
        int32_t top;
        int32_t right;
        int32_t bottom;
    };
    static_assert(sizeof(C2DBoxI) == 0x10);

    struct SearchResultBuffer
    {
        void** begin;
        void** end;
        void** capacity;
    };
    static_assert(offsetof(SearchResultBuffer, begin) == 0x00);
    static_assert(offsetof(SearchResultBuffer, end) == 0x04);
    static_assert(offsetof(SearchResultBuffer, capacity) == 0x08);
    static_assert(sizeof(SearchResultBuffer) == 0x0C);

    struct CScriptThingOverlay
    {
        void* vfptr;
        uint32_t value4;
        void* shared;
    };
    static_assert(offsetof(CScriptThingOverlay, vfptr) == 0x00);
    static_assert(offsetof(CScriptThingOverlay, value4) == 0x04);
    static_assert(offsetof(CScriptThingOverlay, shared) == 0x08);
    static_assert(sizeof(CScriptThingOverlay) == 0x0C);

    struct SharedThingOverlay
    {
        int32_t refCount;
        void* destroySlot;
        void* destroyContext;
    };
    static_assert(offsetof(SharedThingOverlay, refCount) == 0x00);
    static_assert(offsetof(SharedThingOverlay, destroySlot) == 0x04);
    static_assert(offsetof(SharedThingOverlay, destroyContext) == 0x08);

    struct CGameScriptInterfaceField4Overlay
    {
        std::byte pad_00[0x20];
        CThingSearchTools* pThingSearchTools;
    };
    static_assert(offsetof(CGameScriptInterfaceField4Overlay, pThingSearchTools) == 0x20);

    struct CGameScriptInterfaceOverlay
    {
        std::byte pad_00[0x04];
        CGameScriptInterfaceField4Overlay* field_04;
    };
    static_assert(offsetof(CGameScriptInterfaceOverlay, field_04) == 0x04);

    using DestroySlotFn = void(__thiscall*)(void*);
}

int32_t CGameScriptInterface::GetAllCreaturesInAreaWithScriptName(
    const CCharString& scriptName,
    const C3DVector& position,
    float radius,
    std::vector<CScriptThing>& outThings) const
{
    const int32_t x = __ftol2(position.x);
    const int32_t y = __ftol2(position.y);
    int32_t r = __ftol2(radius);

    C2DBoxI roughArea{};
    roughArea.left = x - r;
    roughArea.top = y - r;
    roughArea.right = x + r;
    roughArea.bottom = y + r;

    SearchResultBuffer matches{};
    alignas(CScriptThingOverlay) std::byte tempStorage[sizeof(CScriptThingOverlay)]{};
    auto& tempThing = reinterpret_cast<CScriptThingOverlay&>(tempStorage[0]);

    const auto* const self = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);
    CThingSearchTools* const searchTools = self->field_04->pThingSearchTools;

    bool usePotentialCustomerFilter;
    if (*reinterpret_cast<CBasicString<char>* const*>(&scriptName) == nullptr)
    {
        int count = 1;
        bool equal = true;
        const char* lhs = "";
        const char* rhs = "";
        do
        {
            if (count == 0)
            {
                break;
            }
            --count;
            equal = (*lhs == *rhs);
            ++lhs;
            ++rhs;
        } while (equal);
        usePotentialCustomerFilter = equal;
    }
    else
    {
        usePotentialCustomerFilter =
            CBasicString<char>::operator==(*reinterpret_cast<CBasicString<char>* const*>(&scriptName), "");
    }

    if (usePotentialCustomerFilter)
    {
        CThingSearchTools::GetPBestThingInRoughArea<
            CThingFilter_IsPotentialCustomer,
            CThingCompare_Nearest,
            CThingCreatureBase>(
                searchTools,
                roughArea,
                reinterpret_cast<const CThingFilter_IsPotentialCustomer*>(&position),
                reinterpret_cast<CThingCompare_Nearest*>(&matches));
    }
    else
    {
        CCharString copiedScriptName(scriptName);

        CThingSearchTools::GetPBestThingInRoughArea<
            CFilter_Bind2<CThingFilter_IsNot, CIsVeryCloseMotionlessCreature>,
            CThingCompare_Nearest,
            CThingCreatureBase>(
                searchTools,
                roughArea,
                reinterpret_cast<const CFilter_Bind2<CThingFilter_IsNot, CIsVeryCloseMotionlessCreature>*>(&copiedScriptName),
                reinterpret_cast<CThingCompare_Nearest*>(&matches));

        std::_Cons_val<
            std::allocator<std::pair<EHeroMorphType, CParticleMorphs::CEntry>>,
            std::pair<EHeroMorphType, CParticleMorphs::CEntry>,
            const std::pair<EHeroMorphType, CParticleMorphs::CEntry>&>(
                reinterpret_cast<std::allocator<std::pair<EHeroMorphType, CParticleMorphs::CEntry>>*>(&copiedScriptName));
    }

    if (matches.begin == matches.end)
    {
        if (matches.begin != nullptr)
        {
            free(matches.begin);
        }
        return 0;
    }

    std__vector___Reserve(
        &outThings,
        static_cast<int32_t>(
            (reinterpret_cast<uintptr_t>(matches.end) - reinterpret_cast<uintptr_t>(matches.begin)) >> 2));

    for (void** it = matches.begin; it != matches.end; ++it)
    {
        const int32_t producedThing = NScript::GFPredicateAnd<
            NScript::CPredicate_And<
                NScript::CPredicate_And<
                    NScript::CPredicate_And<
                        NScript::CIsThingAlive,
                        NScript::CIsThingAlive>,
                    NScript::CIsThingAlive>,
                NScript::CIsThingAlive>,
            NScript::CIsThingAlive>(
                reinterpret_cast<NScript::CPredicate_And<
                    NScript::CPredicate_And<
                        NScript::CPredicate_And<
                            NScript::CIsThingAlive,
                            NScript::CIsThingAlive>,
                        NScript::CIsThingAlive>,
                    NScript::CIsThingAlive>*>(*it),
                reinterpret_cast<CIsThingAlive*>(&tempThing));

        auto* const dst = reinterpret_cast<CScriptThingOverlay*>(outThings._Mylast);
        if (dst == reinterpret_cast<CScriptThingOverlay*>(outThings._Myend))
        {
            std__vector_InsertRange(dst, producedThing, &tempThing, 1, 1);
        }
        else
        {
            if (dst != nullptr)
            {
                dst->vfptr = reinterpret_cast<void*>(0x01238C8C);
                dst->value4 = *reinterpret_cast<uint32_t*>(producedThing + 4);
                auto* const shared = *reinterpret_cast<SharedThingOverlay**>(producedThing + 8);
                dst->shared = shared;
                if (shared != nullptr)
                {
                    ++shared->refCount;
                }
            }

            outThings._Mylast =
                reinterpret_cast<CScriptThing*>(reinterpret_cast<std::byte*>(outThings._Mylast) + sizeof(CScriptThingOverlay));
        }

        tempThing.vfptr = reinterpret_cast<void*>(0x01238C8C);
        if (tempThing.shared != nullptr)
        {
            auto* const shared = static_cast<SharedThingOverlay*>(tempThing.shared);
            --shared->refCount;
            if (shared->refCount == 0)
            {
                void* const destroyContext = shared->destroyContext;
                reinterpret_cast<DestroySlotFn>(shared->destroySlot)(destroyContext);
                operator delete(shared);
            }
        }
        tempThing.value4 = 0;
        tempThing.shared = nullptr;
        NHeroInformationScreens::CBase::CBase(reinterpret_cast<NHeroInformationScreens::CBase*>(&tempThing));
    }

    const int32_t first = reinterpret_cast<int32_t>(outThings._Myfirst);
    const int32_t last = reinterpret_cast<int32_t>(outThings._Mylast);

    if (matches.begin != nullptr)
    {
        free(matches.begin);
    }

    return (last - first) / 0x0C;
}
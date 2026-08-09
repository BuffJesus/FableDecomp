namespace
{
    struct CGameScriptInterface_SearchContextOverlay
    {
        std::byte pad00[0x20];
        CThingSearchTools* pThingSearchTools;
    };
    static_assert(offsetof(CGameScriptInterface_SearchContextOverlay, pThingSearchTools) == 0x20);

    struct CGameScriptInterface_LayoutOverlay
    {
        std::byte pad00[0x04];
        CGameScriptInterface_SearchContextOverlay* pSearchContext;
    };
    static_assert(offsetof(CGameScriptInterface_LayoutOverlay, pSearchContext) == 0x04);

    struct CCharString_LayoutOverlay
    {
        CBasicString<char>* pBasicString;
    };
    static_assert(offsetof(CCharString_LayoutOverlay, pBasicString) == 0x00);

    struct PotentialCustomerFilterStackOverlay
    {
        const C3DVector* pPosition;
        float radius;
    };
    static_assert(offsetof(PotentialCustomerFilterStackOverlay, pPosition) == 0x00);
    static_assert(offsetof(PotentialCustomerFilterStackOverlay, radius) == 0x04);

    struct CScriptThingSharedOverlay
    {
        std::int32_t refCount;
        void (__thiscall* pDestroy)(CScriptThingSharedOverlay*);
    };
    static_assert(offsetof(CScriptThingSharedOverlay, refCount) == 0x00);
    static_assert(offsetof(CScriptThingSharedOverlay, pDestroy) == 0x04);

    struct CScriptThingResultOverlay
    {
        void* pVftable;
        std::int32_t value;
        CScriptThingSharedOverlay* pShared;
    };
    static_assert(sizeof(CScriptThingResultOverlay) == 0x0C);
    static_assert(offsetof(CScriptThingResultOverlay, pVftable) == 0x00);
    static_assert(offsetof(CScriptThingResultOverlay, value) == 0x04);
    static_assert(offsetof(CScriptThingResultOverlay, pShared) == 0x08);

    struct CScriptThingVectorOverlay
    {
        CScriptThing* pBegin;
        CScriptThing* pEnd;
        CScriptThing* pCapacityEnd;
    };
    static_assert(sizeof(CScriptThingVectorOverlay) == 0x0C);
    static_assert(offsetof(CScriptThingVectorOverlay, pBegin) == 0x00);
    static_assert(offsetof(CScriptThingVectorOverlay, pEnd) == 0x04);
    static_assert(offsetof(CScriptThingVectorOverlay, pCapacityEnd) == 0x08);

    struct CreaturePointerVectorOverlay
    {
        CThingCreatureBase** pBegin;
        CThingCreatureBase** pEnd;
        CThingCreatureBase** pCapacityEnd;
    };
    static_assert(sizeof(CreaturePointerVectorOverlay) == 0x0C);
    static_assert(offsetof(CreaturePointerVectorOverlay, pBegin) == 0x00);
    static_assert(offsetof(CreaturePointerVectorOverlay, pEnd) == 0x04);
    static_assert(offsetof(CreaturePointerVectorOverlay, pCapacityEnd) == 0x08);

    union Param3ReuseOverlay
    {
        PotentialCustomerFilterStackOverlay potentialCustomerFilter;
        alignas(CCharString) std::byte charStringStorage[sizeof(CCharString)];
    };

    extern void* PTR__scalar_deleting_destructor__01238c8c;

    void __thiscall std__vector___Reserve(CScriptThingVectorOverlay* self, int appendCount);
    void __thiscall std__vector_InsertRange(
        CScriptThingVectorOverlay* self,
        void* pInsertionPoint,
        const void* pSourceObject,
        void* pStackScratch,
        int count,
        int unknownOne);

    inline std::int32_t Ftol2(float value)
    {
        return static_cast<std::int32_t>(value);
    }
}

long CGameScriptInterface::GetAllCreaturesInAreaWithScriptName(
    const CCharString& scriptName,
    const C3DVector& position,
    float radius,
    std::vector<CScriptThing>& outThings) const
{
    const auto* const self = reinterpret_cast<const CGameScriptInterface_LayoutOverlay*>(this);
    const auto* const scriptNameLayout = reinterpret_cast<const CCharString_LayoutOverlay*>(&scriptName);

    const std::int32_t xInt = Ftol2(position.x);
    const std::int32_t yInt = Ftol2(position.y);
    std::int32_t radiusInt = Ftol2(radius);

    std::int32_t left = xInt - radiusInt;
    std::int32_t top = yInt - radiusInt;
    std::int32_t right = xInt + radiusInt;
    std::int32_t bottom = yInt + radiusInt;

    C2DBoxI roughArea{};
    roughArea.left = left;
    roughArea.top = top;
    roughArea.right = right;
    roughArea.bottom = bottom;

    CreaturePointerVectorOverlay matches{};
    matches.pBegin = nullptr;
    matches.pEnd = nullptr;
    matches.pCapacityEnd = nullptr;

    CScriptThingResultOverlay thingTemp{};
    thingTemp.pVftable = nullptr;
    thingTemp.value = 0;
    thingTemp.pShared = nullptr;

    Param3ReuseOverlay param3Reuse{};

    if (scriptNameLayout->pBasicString == nullptr)
    {
        int compareCount = 1;
        bool equal = true;
        const char* lhs = "";
        const char* rhs = "";

        do
        {
            if (compareCount == 0)
            {
                break;
            }

            compareCount = compareCount - 1;
            equal = (*lhs == *rhs);
            lhs = lhs + 1;
            rhs = rhs + 1;
        } while (equal);

        if (equal)
        {
goto_empty_name_search:
            param3Reuse.potentialCustomerFilter.pPosition = &position;
            param3Reuse.potentialCustomerFilter.radius = radius;

            CThingSearchTools::GetPBestThingInRoughArea<
                CThingFilter_IsPotentialCustomer,
                CThingCompare_Nearest,
                CThingCreatureBase>(
                self->pSearchContext->pThingSearchTools,
                &roughArea,
                reinterpret_cast<CThingFilter_IsPotentialCustomer*>(&param3Reuse),
                reinterpret_cast<CThingCompare_Nearest*>(&matches),
                static_cast<CThingCreatureBase**>(nullptr));
            goto_search_done;
        }
    }
    else
    {
        const bool isEmpty = CBasicString<char>::operator==(scriptNameLayout->pBasicString, "");
        if (isEmpty)
        {
            goto goto_empty_name_search;
        }
    }

    CCharString::CCharString(reinterpret_cast<CCharString*>(&param3Reuse), &scriptName);
    CThingSearchTools::GetPBestThingInRoughArea<
        CFilter_Bind2<CThingFilter_IsNot, CIsVeryCloseMotionlessCreature>,
        CThingCompare_Nearest,
        CThingCreatureBase>(
        self->pSearchContext->pThingSearchTools,
        &roughArea,
        reinterpret_cast<CFilter_Bind2<CThingFilter_IsNot, CIsVeryCloseMotionlessCreature>*>(&param3Reuse),
        reinterpret_cast<CThingCompare_Nearest*>(&matches),
        static_cast<CThingCreatureBase**>(nullptr));
    reinterpret_cast<CCharString*>(&param3Reuse)->~CCharString();

goto_search_done:
    CThingCreatureBase** const matchEnd = matches.pEnd;
    CThingCreatureBase** const matchBegin = matches.pBegin;
    auto* const outVector = reinterpret_cast<CScriptThingVectorOverlay*>(&outThings);

    if (matchBegin == matchEnd)
    {
        if (matchBegin != nullptr)
        {
            free(matchBegin);
        }
        return 0;
    }

    std__vector___Reserve(outVector, static_cast<int>(matchEnd - matchBegin));

    CThingCreatureBase** current = matchBegin;
    do
    {
        const auto* const resultObject =
            reinterpret_cast<const CScriptThingResultOverlay*>(
                NScript::GFPredicateAnd<
                    NScript::CPredicate_And<
                        NScript::CPredicate_And<
                            NScript::CPredicate_And<
                                NScript::CIsThingAlive,
                                NScript::CIsThingAlive>,
                            NScript::CIsThingAlive>,
                        NScript::CIsThingAlive>,
                    NScript::CIsThingAlive>(
                    *current,
                    reinterpret_cast<CIsThingAlive*>(&thingTemp)));

        void* const insertionPoint = outVector->pEnd;
        if (insertionPoint == outVector->pCapacityEnd)
        {
            std__vector_InsertRange(
                outVector,
                insertionPoint,
                resultObject,
                &param3Reuse,
                1,
                1);
        }
        else
        {
            auto* const dest = reinterpret_cast<CScriptThingResultOverlay*>(insertionPoint);
            if (dest != nullptr)
            {
                dest->pVftable = &PTR__scalar_deleting_destructor__01238c8c;
                dest->value = resultObject->value;
                dest->pShared = resultObject->pShared;
                if (dest->pShared != nullptr)
                {
                    dest->pShared->refCount = dest->pShared->refCount + 1;
                }
            }
            outVector->pEnd = reinterpret_cast<CScriptThing*>(
                reinterpret_cast<std::byte*>(outVector->pEnd) + 0x0C);
        }

        thingTemp.pVftable = &PTR__scalar_deleting_destructor__01238c8c;
        if (thingTemp.pShared != nullptr)
        {
            thingTemp.pShared->refCount = thingTemp.pShared->refCount - 1;
            if (thingTemp.pShared->refCount == 0)
            {
                thingTemp.pShared->pDestroy(thingTemp.pShared);
                operator delete(thingTemp.pShared);
            }
        }

        thingTemp.value = 0;
        thingTemp.pShared = nullptr;
        NHeroInformationScreens::CBase::CBase(
            reinterpret_cast<NHeroInformationScreens::CBase*>(&thingTemp));

        current = current + 1;
    } while (current != matchEnd);

    const long result = static_cast<long>(
        (reinterpret_cast<std::byte*>(outVector->pEnd) -
         reinterpret_cast<std::byte*>(outVector->pBegin)) / 0x0C);

    if (matchBegin != nullptr)
    {
        free(matchBegin);
    }

    return result;
}
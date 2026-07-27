int CGameScriptInterface::GetAllCreaturesExcludingHero(std::vector<CScriptThing>& out) const
{
    struct SearchOwnerOverlay
    {
        std::byte pad_00[0x20];
        CThingSearchTools* thingSearchTools;
    };
    static_assert(offsetof(SearchOwnerOverlay, thingSearchTools) == 0x20);

    struct ThisOverlay
    {
        std::byte pad_00[0x04];
        SearchOwnerOverlay* data;
    };
    static_assert(offsetof(ThisOverlay, data) == 0x04);

    struct ScriptThingRefOverlay
    {
        int refCount;
        void (*releaseFn)();
    };
    static_assert(offsetof(ScriptThingRefOverlay, refCount) == 0x00);
    static_assert(offsetof(ScriptThingRefOverlay, releaseFn) == 0x04);

    struct ScriptThingOverlay
    {
        void* vftable;
        int value04;
        ScriptThingRefOverlay* refObject;
    };
    static_assert(sizeof(ScriptThingOverlay) == 0x0C);
    static_assert(offsetof(ScriptThingOverlay, value04) == 0x04);
    static_assert(offsetof(ScriptThingOverlay, refObject) == 0x08);

    struct ScriptThingVectorOverlay
    {
        ScriptThingOverlay* myFirst;
        ScriptThingOverlay* myLast;
        ScriptThingOverlay* myEnd;
    };
    static_assert(sizeof(ScriptThingVectorOverlay) == 0x0C);
    static_assert(offsetof(ScriptThingVectorOverlay, myFirst) == 0x00);
    static_assert(offsetof(ScriptThingVectorOverlay, myLast) == 0x04);
    static_assert(offsetof(ScriptThingVectorOverlay, myEnd) == 0x08);

    CThingSearchTools* const thingSearchTools =
        reinterpret_cast<const ThisOverlay*>(this)->data->thingSearchTools;

    CThingAICreature** creatureFirst = nullptr;
    CThingAICreature** creatureLast = nullptr;
    CThingAICreature** creatureEnd = nullptr;
    (void)creatureEnd;

    CThingSearchTools::GetAllThings<
        CThingAICreature,
        std::vector<CThingAICreature*, std::allocator<CThingAICreature*>>,
        CFilter_Bind2<CThingFilter_None, CThingFilter_HasTC>
    >(
        thingSearchTools,
        reinterpret_cast<CFilter_Bind2<CThingFilter_None, CThingFilter_HasTC>*>(&out),
        reinterpret_cast<std::vector<CThingAICreature*, std::allocator<CThingAICreature*>>&>(creatureFirst));

    ScriptThingOverlay temp{};
    ScriptThingVectorOverlay& outOverlay = reinterpret_cast<ScriptThingVectorOverlay&>(out);

    const unsigned int creatureCount =
        static_cast<unsigned int>((reinterpret_cast<int>(creatureLast) - reinterpret_cast<int>(creatureFirst)) >> 2);

    std__vector___Reserve(&out, creatureCount);

    for (unsigned int i = 0; i < creatureCount; ++i)
    {
        const int scriptThingResult =
            NScript::GFPredicateAnd<
                NScript::CPredicate_And<
                    NScript::CPredicate_And<
                        NScript::CPredicate_And<
                            NScript::CIsThingAlive,
                            NScript::CIsThingAlive
                        >,
                        NScript::CIsThingAlive
                    >,
                    NScript::CIsThingAlive
                >,
                NScript::CIsThingAlive
            >(
                creatureFirst[i],
                reinterpret_cast<NScript::CIsThingAlive*>(&temp));

        ScriptThingOverlay* const appendAt = outOverlay.myLast;
        if (appendAt == outOverlay.myEnd)
        {
            std__vector_InsertRange(appendAt, scriptThingResult, &out, 1, 1);
        }
        else
        {
            if (appendAt != nullptr)
            {
                appendAt->vftable = &PTR__scalar_deleting_destructor__01238c8c;
                appendAt->value04 = *reinterpret_cast<int*>(scriptThingResult + 0x04);

                ScriptThingRefOverlay* const refObject =
                    *reinterpret_cast<ScriptThingRefOverlay**>(scriptThingResult + 0x08);
                appendAt->refObject = refObject;

                if (refObject != nullptr)
                {
                    refObject->refCount = refObject->refCount + 1;
                }
            }

            outOverlay.myLast =
                reinterpret_cast<ScriptThingOverlay*>(reinterpret_cast<std::byte*>(outOverlay.myLast) + 0x0C);
        }

        temp.vftable = &PTR__scalar_deleting_destructor__01238c8c;
        if (temp.refObject != nullptr)
        {
            temp.refObject->refCount = temp.refObject->refCount + -1;
            if (temp.refObject->refCount == 0)
            {
                temp.refObject->releaseFn();
                operator delete(temp.refObject);
            }
        }

        temp.value04 = 0;
        temp.refObject = nullptr;
        NHeroInformationScreens::CBase::CBase(reinterpret_cast<NHeroInformationScreens::CBase*>(&temp));
    }

    const int beginBytes = reinterpret_cast<int>(outOverlay.myFirst);
    const int endBytes = reinterpret_cast<int>(outOverlay.myLast);

    if (creatureFirst != nullptr)
    {
        free(creatureFirst);
    }

    return (endBytes - beginBytes) / 0x0C;
}
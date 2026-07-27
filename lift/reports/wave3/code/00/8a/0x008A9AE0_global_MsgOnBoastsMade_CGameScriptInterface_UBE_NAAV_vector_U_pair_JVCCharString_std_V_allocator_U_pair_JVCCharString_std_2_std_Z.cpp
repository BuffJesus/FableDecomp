namespace
{
    struct CGameScriptInterface_ContextOverlay
    {
        std::byte pad_00[0x04];
        void* context;
    };
    static_assert(offsetof(CGameScriptInterface_ContextOverlay, context) == 0x04);

    struct Context_MessageEventManagerOverlay
    {
        std::byte pad_00[0x60];
        CMessageEventManager* messageEventManager;
    };
    static_assert(offsetof(Context_MessageEventManagerOverlay, messageEventManager) == 0x60);

    struct MessageEventListNode
    {
        MessageEventListNode* next;
        MessageEventListNode* prev;
        void* messageEvent;
    };
    static_assert(sizeof(MessageEventListNode) == 0x0C);
    static_assert(offsetof(MessageEventListNode, messageEvent) == 0x08);

    struct BoastExtraData
    {
        long boastId;
        std::byte pad_04[0x08];
        CCharString boastText;
    };
    static_assert(offsetof(BoastExtraData, boastText) == 0x0C);

    struct MessageEvent_BoastOverlay
    {
        std::byte pad_00[0x3C];
        BoastExtraData* const* extraData;
    };
    static_assert(offsetof(MessageEvent_BoastOverlay, extraData) == 0x3C);

    struct MessageFilterTypeState
    {
        int messageType;
        MessageEventListNode* results;
    };
    static_assert(sizeof(MessageFilterTypeState) == 0x08);

    struct MessageFilterBetweenState
    {
        long firstFrame;
        long lastFrame;
    };
    static_assert(sizeof(MessageFilterBetweenState) == 0x08);

    struct MessageFilterBind3State
    {
        MessageFilterTypeState* isOfType;
        MessageFilterBetweenState* occuredBetween;
    };
    static_assert(sizeof(MessageFilterBind3State) == 0x08);

    using BoastPair = std::pair<long, CCharString>;

    struct BoastVectorOverlay
    {
        BoastPair* first;
        BoastPair* last;
        BoastPair* end;
    };
    static_assert(offsetof(BoastVectorOverlay, first) == 0x00);
    static_assert(offsetof(BoastVectorOverlay, last) == 0x04);
    static_assert(offsetof(BoastVectorOverlay, end) == 0x08);

    struct LocalPairRangeOverlay
    {
        long local_20;
        CCharString local_1c;
        std::byte local_29;
    };
    static_assert(offsetof(LocalPairRangeOverlay, local_20) == 0x00);
    static_assert(offsetof(LocalPairRangeOverlay, local_1c) == 0x04);

    extern BoastPair* __thiscall std__vector_InsertRangeWithCopy(
        BoastVectorOverlay* self,
        BoastPair* position,
        const void* first,
        const void* last,
        int count,
        int one_more);

    extern BoastPair* __thiscall CCharString_CopyConstruct(
        CCharString* self,
        const CCharString* source);

    extern void __thiscall std___Cons_val__allocator_pair_long_CCharString__(
        CCharString* self,
        BoastPair* ppVar6,
        const BoastPair* unaff_EDI);
}

bool CGameScriptInterface::MsgOnBoastsMade(std::vector<std::pair<long, CCharString>>& out) const
{
    const long firstFrame = this->GetMaxWorldFrameForMessages();
    const long lastFrame = this->GetMaxWorldFrameForMessages();

    auto* const context =
        reinterpret_cast<const CGameScriptInterface_ContextOverlay*>(this)->context;
    auto* const messageEventManager =
        reinterpret_cast<Context_MessageEventManagerOverlay*>(context)->messageEventManager;

    auto* const listHead =
        static_cast<MessageEventListNode*>(std::malloc(sizeof(MessageEventListNode)));
    listHead->next = listHead;
    listHead->prev = listHead;

    MessageFilterTypeState typeState{0x29, listHead};
    MessageFilterBetweenState betweenState{firstFrame, lastFrame};
    MessageFilterBind3State filterState{&typeState, &betweenState};

    messageEventManager
        ->FindMostRecentMessage<
            CFilter_Bind3<
                CMessageEventFilter_IsOfType,
                CMessageEventFilter_HasExtraData,
                CMessageEventFilter_OccuredBetween>>(
            reinterpret_cast<
                CFilter_Bind3<
                    CMessageEventFilter_IsOfType,
                    CMessageEventFilter_HasExtraData,
                    CMessageEventFilter_OccuredBetween>*>(&filterState));

    for (MessageEventListNode* node = listHead->next; node != listHead; node = node->next)
    {
        auto* const messageEvent = reinterpret_cast<MessageEvent_BoastOverlay*>(node->messageEvent);
        BoastExtraData* const extraData = *messageEvent->extraData;

        LocalPairRangeOverlay localPairRange;
        localPairRange.local_20 = extraData->boastId;
        CCharString_CopyConstruct(&localPairRange.local_1c, &extraData->boastText);

        auto* const rawOut = reinterpret_cast<BoastVectorOverlay*>(&out);
        BoastPair* ppVar6;
        const BoastPair* unaff_EDI;

        BoastPair* const puVar4 = rawOut->last;
        if (puVar4 == rawOut->end)
        {
            ppVar6 = std__vector_InsertRangeWithCopy(
                rawOut,
                puVar4,
                &localPairRange.local_20,
                &localPairRange.local_29,
                1,
                1);
        }
        else
        {
            ppVar6 = puVar4;
            if (puVar4 != nullptr)
            {
                puVar4->first = localPairRange.local_20;
                ppVar6 = CCharString_CopyConstruct(&puVar4->second, &localPairRange.local_1c);
            }
            rawOut->last = reinterpret_cast<BoastPair*>(
                reinterpret_cast<std::byte*>(rawOut->last) + sizeof(BoastPair));
        }

        std___Cons_val__allocator_pair_long_CCharString__(
            &localPairRange.local_1c,
            ppVar6,
            unaff_EDI);
    }

    MessageEventListNode* node = listHead->next;
    if (node != listHead)
    {
        int count = 0;
        MessageEventListNode* scan = node;
        do
        {
            scan = scan->next;
            ++count;
        } while (scan != listHead);

        if (count != 0)
        {
            while (node != listHead)
            {
                MessageEventListNode* const next = node->next;
                std::free(node);
                node = next;
            }

            listHead->next = listHead;
            listHead->prev = listHead;
            std::free(listHead);
            return true;
        }
    }

    while (node != listHead)
    {
        MessageEventListNode* const next = node->next;
        std::free(node);
        node = next;
    }

    listHead->next = listHead;
    listHead->prev = listHead;
    std::free(listHead);
    return false;
}
struct CGameScriptInterface_OwnerOverlay
{
    std::byte pad_00[0x60];
    CMessageEventManager* messageEventManager;
};
static_assert(offsetof(CGameScriptInterface_OwnerOverlay, messageEventManager) == 0x60);

struct CGameScriptInterface_ThisOverlay
{
    std::byte pad_00[0x04];
    CGameScriptInterface_OwnerOverlay* owner;
};
static_assert(offsetof(CGameScriptInterface_ThisOverlay, owner) == 0x04);

struct MsgOnBoastsMade_ListNode
{
    MsgOnBoastsMade_ListNode* next;
    MsgOnBoastsMade_ListNode* prev;
    void* messageEvent;
};
static_assert(offsetof(MsgOnBoastsMade_ListNode, next) == 0x00);
static_assert(offsetof(MsgOnBoastsMade_ListNode, prev) == 0x04);
static_assert(offsetof(MsgOnBoastsMade_ListNode, messageEvent) == 0x08);
static_assert(sizeof(MsgOnBoastsMade_ListNode) == 0x0C);

struct MsgOnBoastsMade_MessageEventOverlay
{
    std::byte pad_00[0x3C];
    void* extraData;
};
static_assert(offsetof(MsgOnBoastsMade_MessageEventOverlay, extraData) == 0x3C);

struct MsgOnBoastsMade_ExtraDataOverlay
{
    long boastId;
    std::byte pad_04[0x08];
    CCharString boastText;
};
static_assert(offsetof(MsgOnBoastsMade_ExtraDataOverlay, boastText) == 0x0C);

struct MsgOnBoastsMade_FilterIsOfType
{
    long type;
};

struct MsgOnBoastsMade_FilterHasExtraData
{
    MsgOnBoastsMade_ListNode* listHead;
};

struct MsgOnBoastsMade_FilterOccuredBetween
{
    long firstFrame;
    long lastFrame;
};

template <typename T1, typename T2, typename T3>
struct MsgOnBoastsMade_FilterBind3
{
    T1* first;
    T2* second;
    T3* third;
};

struct MsgOnBoastsMade_VectorOverlay
{
    void* start;
    void* finish;
    void* endOfStorage;
};
static_assert(offsetof(MsgOnBoastsMade_VectorOverlay, finish) == 0x04);
static_assert(offsetof(MsgOnBoastsMade_VectorOverlay, endOfStorage) == 0x08);

struct MsgOnBoastsMade_PairOpaque;

extern MsgOnBoastsMade_PairOpaque* __thiscall std__vector_InsertRangeWithCopy(
    void* vectorThis,
    void* insertAt,
    const void* first,
    const void* last,
    int count,
    int copyConstruct);

extern MsgOnBoastsMade_PairOpaque* __thiscall CCharString_CopyConstruct_ReturnEdx(
    CCharString* dst,
    const CCharString& src);

extern void __thiscall std___Cons_val_pair_long_CCharString__(
    void* local_1c_as_allocator,
    MsgOnBoastsMade_PairOpaque* ppVar6,
    void* unaff_EDI);

bool CGameScriptInterface::MsgOnBoastsMade(
    std::vector<std::pair<long, CCharString>>& out) const
{
    const long firstFrame = CGameScriptInterface::GetMaxWorldFrameForMessages();
    const long lastFrame = CGameScriptInterface::GetMaxWorldFrameForMessages();

    CMessageEventManager* const messageEventManager =
        reinterpret_cast<const CGameScriptInterface_ThisOverlay*>(this)->owner->messageEventManager;

    auto* const listHead =
        static_cast<MsgOnBoastsMade_ListNode*>(std::malloc(sizeof(MsgOnBoastsMade_ListNode)));
    listHead->next = listHead;
    listHead->prev = listHead;

    MsgOnBoastsMade_FilterIsOfType isOfType{0x29};
    MsgOnBoastsMade_FilterHasExtraData hasExtraData{listHead};
    MsgOnBoastsMade_FilterOccuredBetween occuredBetween{firstFrame, lastFrame};
    MsgOnBoastsMade_FilterBind3<
        MsgOnBoastsMade_FilterIsOfType,
        MsgOnBoastsMade_FilterHasExtraData,
        MsgOnBoastsMade_FilterOccuredBetween>
        filter{&isOfType, &hasExtraData, &occuredBetween};

    messageEventManager->FindMostRecentMessage<
        MsgOnBoastsMade_FilterBind3<
            MsgOnBoastsMade_FilterIsOfType,
            MsgOnBoastsMade_FilterHasExtraData,
            MsgOnBoastsMade_FilterOccuredBetween>>>(&filter);

    auto* const outOverlay = reinterpret_cast<MsgOnBoastsMade_VectorOverlay*>(&out);

    for (auto* node = listHead->next; node != listHead; node = node->next)
    {
        auto* const messageEvent =
            static_cast<MsgOnBoastsMade_MessageEventOverlay*>(node->messageEvent);
        auto* const extraData =
            static_cast<MsgOnBoastsMade_ExtraDataOverlay*>(messageEvent->extraData);

        long local_20 = extraData->boastId;
        CCharString local_1c(extraData->boastText);

        MsgOnBoastsMade_PairOpaque* ppVar6;
        void* const finish = outOverlay->finish;

        if (finish == outOverlay->endOfStorage)
        {
            alignas(long) struct
            {
                long local_20;
                CCharString local_1c;
                unsigned char local_29;
            } temp{local_20, local_1c, 0};

            ppVar6 = std__vector_InsertRangeWithCopy(
                outOverlay,
                finish,
                &temp.local_20,
                &temp.local_29,
                1,
                1);
        }
        else
        {
            ppVar6 = reinterpret_cast<MsgOnBoastsMade_PairOpaque*>(finish);

            if (finish != nullptr)
            {
                *static_cast<long*>(finish) = local_20;
                ppVar6 = CCharString_CopyConstruct_ReturnEdx(
                    reinterpret_cast<CCharString*>(static_cast<std::byte*>(finish) + 4),
                    local_1c);
            }

            outOverlay->finish = static_cast<std::byte*>(finish) + 8;
        }

        std___Cons_val_pair_long_CCharString__(&local_1c, ppVar6, __builtin_frame_address(0));
    }

    auto* node = listHead->next;
    if (node != listHead)
    {
        int count = 0;
        auto* cursor = node;
        do
        {
            cursor = cursor->next;
            ++count;
        } while (cursor != listHead);

        if (count != 0)
        {
            while (node != listHead)
            {
                auto* const next = node->next;
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
        auto* const next = node->next;
        std::free(node);
        node = next;
    }

    listHead->next = listHead;
    listHead->prev = listHead;
    std::free(listHead);
    return false;
}
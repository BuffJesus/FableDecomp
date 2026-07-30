#include <cstddef>
#include <cstdlib>
#include <vector>

class CCharString;
class CMessageEventManager;
class CGameScriptInterface;

struct CMessageEventFilter_IsOfType;
struct CMessageEventFilter_HasExtraData;
struct CMessageEventFilter_OccuredBetween;

template <typename T0, typename T1, typename T2>
struct CFilter_Bind3;

namespace
{
struct MsgOnBoastsMade_ManagerOwnerOverlay
{
    std::byte pad[0x60];
    CMessageEventManager* messageEventManager;
};
static_assert(offsetof(MsgOnBoastsMade_ManagerOwnerOverlay, messageEventManager) == 0x60);

struct MsgOnBoastsMade_ScriptInterfaceOverlay
{
    std::byte pad[0x04];
    MsgOnBoastsMade_ManagerOwnerOverlay* owner;
};
static_assert(offsetof(MsgOnBoastsMade_ScriptInterfaceOverlay, owner) == 0x04);

struct MsgOnBoastsMade_EventPayloadOverlay
{
    long boastId;
    std::byte pad[0x08];
    CCharString boastText;
};
static_assert(offsetof(MsgOnBoastsMade_EventPayloadOverlay, boastText) == 0x0C);

struct MsgOnBoastsMade_MessageEventOverlay
{
    std::byte pad[0x3C];
    MsgOnBoastsMade_EventPayloadOverlay* payload;
};
static_assert(offsetof(MsgOnBoastsMade_MessageEventOverlay, payload) == 0x3C);

struct MsgOnBoastsMade_ResultNode
{
    MsgOnBoastsMade_ResultNode* next;
    MsgOnBoastsMade_ResultNode* prev;
    MsgOnBoastsMade_MessageEventOverlay* messageEvent;
};
static_assert(sizeof(MsgOnBoastsMade_ResultNode) == 0x0C);
static_assert(offsetof(MsgOnBoastsMade_ResultNode, messageEvent) == 0x08);

struct MsgOnBoastsMade_TypeAndExtraDataFilterStorage
{
    int messageType;
    MsgOnBoastsMade_ResultNode* resultList;
};
static_assert(sizeof(MsgOnBoastsMade_TypeAndExtraDataFilterStorage) == 0x08);
static_assert(offsetof(MsgOnBoastsMade_TypeAndExtraDataFilterStorage, resultList) == 0x04);

struct MsgOnBoastsMade_OccuredBetweenFilterStorage
{
    long minFrame;
    long maxFrame;
};
static_assert(sizeof(MsgOnBoastsMade_OccuredBetweenFilterStorage) == 0x08);
static_assert(offsetof(MsgOnBoastsMade_OccuredBetweenFilterStorage, maxFrame) == 0x04);

struct MsgOnBoastsMade_FilterBind3Overlay
{
    MsgOnBoastsMade_TypeAndExtraDataFilterStorage* typeAndExtraData;
    MsgOnBoastsMade_OccuredBetweenFilterStorage* occuredBetween;
};
static_assert(sizeof(MsgOnBoastsMade_FilterBind3Overlay) == 0x08);
static_assert(offsetof(MsgOnBoastsMade_FilterBind3Overlay, occuredBetween) == 0x04);

struct MsgOnBoastsMade_AppendedValueOverlay
{
    long boastId;
    CCharString boastText;
};
static_assert(offsetof(MsgOnBoastsMade_AppendedValueOverlay, boastText) == 0x04);
static_assert(sizeof(MsgOnBoastsMade_AppendedValueOverlay) == 0x08);

struct MsgOnBoastsMade_VectorOverlay
{
    MsgOnBoastsMade_AppendedValueOverlay* begin;
    MsgOnBoastsMade_AppendedValueOverlay* finish;
    MsgOnBoastsMade_AppendedValueOverlay* endOfStorage;
};
static_assert(offsetof(MsgOnBoastsMade_VectorOverlay, finish) == 0x04);
static_assert(offsetof(MsgOnBoastsMade_VectorOverlay, endOfStorage) == 0x08);
}

extern MsgOnBoastsMade_AppendedValueOverlay* std__vector_InsertRangeWithCopy(
    MsgOnBoastsMade_AppendedValueOverlay* insertAt,
    const void* first,
    const void* last,
    int constructForward,
    int copyRange);

extern MsgOnBoastsMade_AppendedValueOverlay* CCharString__CopyConstruct_ReturnsEDX(
    CCharString* destination,
    const CCharString* source);

extern void std___Cons_val__allocator_pair_long_CCharString___pair_long_CCharString___const_ref(
    void* allocatorLike,
    MsgOnBoastsMade_AppendedValueOverlay* constructedPair,
    const void* unaff_EDI);

bool CGameScriptInterface::MsgOnBoastsMade(std::vector<std::pair<long, CCharString>>& outBoasts) const
{
    const long minFrame = CGameScriptInterface::GetMaxWorldFrameForMessages();
    const long maxFrame = CGameScriptInterface::GetMaxWorldFrameForMessages();

    const auto* const thisOverlay = reinterpret_cast<const MsgOnBoastsMade_ScriptInterfaceOverlay*>(this);
    CMessageEventManager* const messageEventManager = thisOverlay->owner->messageEventManager;

    auto* const sentinel =
        static_cast<MsgOnBoastsMade_ResultNode*>(std::malloc(sizeof(MsgOnBoastsMade_ResultNode)));
    sentinel->next = sentinel;
    sentinel->prev = sentinel;

    MsgOnBoastsMade_TypeAndExtraDataFilterStorage typeAndExtraData;
    MsgOnBoastsMade_OccuredBetweenFilterStorage occuredBetween;
    MsgOnBoastsMade_FilterBind3Overlay filter;

    typeAndExtraData.messageType = 0x29;
    typeAndExtraData.resultList = sentinel;
    occuredBetween.minFrame = minFrame;
    occuredBetween.maxFrame = maxFrame;
    filter.typeAndExtraData = &typeAndExtraData;
    filter.occuredBetween = &occuredBetween;

    messageEventManager->FindMostRecentMessage<
        CFilter_Bind3<
            CMessageEventFilter_IsOfType,
            CMessageEventFilter_HasExtraData,
            CMessageEventFilter_OccuredBetween>>(
        reinterpret_cast<CFilter_Bind3<
            CMessageEventFilter_IsOfType,
            CMessageEventFilter_HasExtraData,
            CMessageEventFilter_OccuredBetween>*>(&filter));

    void* unaff_EDI; // incoming EDI-backed value observed by the decompilation

    for (auto* node = sentinel->next; node != sentinel; node = node->next)
    {
        MsgOnBoastsMade_AppendedValueOverlay appendedValue;
        MsgOnBoastsMade_AppendedValueOverlay* ppVar6;

        appendedValue.boastId = node->messageEvent->payload->boastId;
        CCharString::CCharString(&appendedValue.boastText, &node->messageEvent->payload->boastText);

        auto& outOverlay = reinterpret_cast<MsgOnBoastsMade_VectorOverlay&>(outBoasts);
        MsgOnBoastsMade_AppendedValueOverlay* const finish = outOverlay.finish;

        if (finish == outOverlay.endOfStorage)
        {
            ppVar6 = std__vector_InsertRangeWithCopy(
                finish,
                &appendedValue.boastId,
                reinterpret_cast<const std::byte*>(&appendedValue.boastId) + 0x08,
                1,
                1);
        }
        else
        {
            ppVar6 = reinterpret_cast<MsgOnBoastsMade_AppendedValueOverlay*>(finish);

            if (finish != nullptr)
            {
                finish->boastId = appendedValue.boastId;
                ppVar6 = CCharString__CopyConstruct_ReturnsEDX(
                    &finish->boastText,
                    &appendedValue.boastText);
            }

            outOverlay.finish = reinterpret_cast<MsgOnBoastsMade_AppendedValueOverlay*>(
                reinterpret_cast<std::byte*>(outOverlay.finish) + 0x08);
        }

        std___Cons_val__allocator_pair_long_CCharString___pair_long_CCharString___const_ref(
            &appendedValue.boastText,
            ppVar6,
            unaff_EDI);
    }

    auto* node = sentinel->next;
    if (node != sentinel)
    {
        int count = 0;
        auto* scan = node;
        do
        {
            scan = scan->next;
            ++count;
        } while (scan != sentinel);

        if (count != 0)
        {
            while (node != sentinel)
            {
                auto* const next = node->next;
                std::free(node);
                node = next;
            }

            sentinel->next = sentinel;
            sentinel->prev = sentinel;
            std::free(sentinel);
            return true;
        }
    }

    while (node != sentinel)
    {
        auto* const next = node->next;
        std::free(node);
        node = next;
    }

    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    std::free(sentinel);
    return false;
}
#include <cstddef>
#include <cstdlib>
#include <new>
#include <utility>
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
struct CGameScriptInterface_MsgOnBoastsMade_ContextOverlay
{
    std::byte pad_00[0x60];
    CMessageEventManager* messageEventManager; // +0x60
};
static_assert(offsetof(CGameScriptInterface_MsgOnBoastsMade_ContextOverlay, messageEventManager) == 0x60);

struct CGameScriptInterface_MsgOnBoastsMade_SelfOverlay
{
    std::byte pad_00[0x04];
    CGameScriptInterface_MsgOnBoastsMade_ContextOverlay* context; // +0x04
};
static_assert(offsetof(CGameScriptInterface_MsgOnBoastsMade_SelfOverlay, context) == 0x04);

struct CGameScriptInterface_MsgOnBoastsMade_ExtraDataOverlay
{
    long boastId;          // +0x00
    std::byte pad_04[0x08];
    CCharString boastText; // +0x0C
};
static_assert(offsetof(CGameScriptInterface_MsgOnBoastsMade_ExtraDataOverlay, boastText) == 0x0C);

struct CGameScriptInterface_MsgOnBoastsMade_MessageEventOverlay
{
    std::byte pad_00[0x3C];
    CGameScriptInterface_MsgOnBoastsMade_ExtraDataOverlay* extraData; // +0x3C
};
static_assert(offsetof(CGameScriptInterface_MsgOnBoastsMade_MessageEventOverlay, extraData) == 0x3C);

struct CGameScriptInterface_MsgOnBoastsMade_ListNodeOverlay
{
    CGameScriptInterface_MsgOnBoastsMade_ListNodeOverlay* next; // +0x00
    CGameScriptInterface_MsgOnBoastsMade_ListNodeOverlay* prev; // +0x04
    CGameScriptInterface_MsgOnBoastsMade_MessageEventOverlay* messageEvent; // +0x08
};
static_assert(offsetof(CGameScriptInterface_MsgOnBoastsMade_ListNodeOverlay, messageEvent) == 0x08);
static_assert(sizeof(CGameScriptInterface_MsgOnBoastsMade_ListNodeOverlay) == 0x0C);

struct CGameScriptInterface_MsgOnBoastsMade_TypeAndExtraDataFilterStorage
{
    long messageType;                                                 // +0x00
    CGameScriptInterface_MsgOnBoastsMade_ListNodeOverlay* resultList; // +0x04
};
static_assert(offsetof(CGameScriptInterface_MsgOnBoastsMade_TypeAndExtraDataFilterStorage, resultList) == 0x04);
static_assert(sizeof(CGameScriptInterface_MsgOnBoastsMade_TypeAndExtraDataFilterStorage) == 0x08);

struct CGameScriptInterface_MsgOnBoastsMade_OccuredBetweenFilterStorage
{
    long minFrame; // +0x00
    long maxFrame; // +0x04
};
static_assert(offsetof(CGameScriptInterface_MsgOnBoastsMade_OccuredBetweenFilterStorage, maxFrame) == 0x04);
static_assert(sizeof(CGameScriptInterface_MsgOnBoastsMade_OccuredBetweenFilterStorage) == 0x08);

struct CGameScriptInterface_MsgOnBoastsMade_FilterBind3Overlay
{
    CGameScriptInterface_MsgOnBoastsMade_TypeAndExtraDataFilterStorage* typeAndExtraData; // +0x00
    CGameScriptInterface_MsgOnBoastsMade_OccuredBetweenFilterStorage* occuredBetween;      // +0x04
};
static_assert(offsetof(CGameScriptInterface_MsgOnBoastsMade_FilterBind3Overlay, occuredBetween) == 0x04);
static_assert(sizeof(CGameScriptInterface_MsgOnBoastsMade_FilterBind3Overlay) == 0x08);

using MsgOnBoastsMade_Pair = std::pair<long, CCharString>;

struct CGameScriptInterface_MsgOnBoastsMade_VectorOverlay
{
    MsgOnBoastsMade_Pair* first; // +0x00
    MsgOnBoastsMade_Pair* last;  // +0x04
    MsgOnBoastsMade_Pair* end;   // +0x08
};
static_assert(offsetof(CGameScriptInterface_MsgOnBoastsMade_VectorOverlay, first) == 0x00);
static_assert(offsetof(CGameScriptInterface_MsgOnBoastsMade_VectorOverlay, last) == 0x04);
static_assert(offsetof(CGameScriptInterface_MsgOnBoastsMade_VectorOverlay, end) == 0x08);

struct CGameScriptInterface_MsgOnBoastsMade_LocalSourceRange
{
    long boastId;          // matches local_20
    CCharString boastText; // matches local_1c
    std::byte sentinel;    // matches local_29
};
static_assert(offsetof(CGameScriptInterface_MsgOnBoastsMade_LocalSourceRange, boastText) == 0x04);

extern MsgOnBoastsMade_Pair* std__vector_InsertRangeWithCopy(
    MsgOnBoastsMade_Pair* insertAt,
    const void* first,
    const void* last,
    int count,
    int copyConstruct);
} // namespace

bool CGameScriptInterface::MsgOnBoastsMade(std::vector<std::pair<long, CCharString>>& outBoasts) const
{
    const long minFrame = this->GetMaxWorldFrameForMessages();
    const long maxFrame = this->GetMaxWorldFrameForMessages();

    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_MsgOnBoastsMade_SelfOverlay*>(this);
    CMessageEventManager* const messageEventManager = self->context->messageEventManager;

    auto* const listHead =
        static_cast<CGameScriptInterface_MsgOnBoastsMade_ListNodeOverlay*>(
            std::malloc(sizeof(CGameScriptInterface_MsgOnBoastsMade_ListNodeOverlay)));
    listHead->next = listHead;
    listHead->prev = listHead;

    CGameScriptInterface_MsgOnBoastsMade_TypeAndExtraDataFilterStorage typeAndExtraData;
    CGameScriptInterface_MsgOnBoastsMade_OccuredBetweenFilterStorage occuredBetween;
    CGameScriptInterface_MsgOnBoastsMade_FilterBind3Overlay filter;

    typeAndExtraData.messageType = 0x29;
    typeAndExtraData.resultList = listHead;
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

    auto& outOverlay =
        reinterpret_cast<CGameScriptInterface_MsgOnBoastsMade_VectorOverlay&>(outBoasts);

    for (auto* node = listHead->next; node != listHead; node = node->next)
    {
        auto* const extraData = node->messageEvent->extraData;

        alignas(CGameScriptInterface_MsgOnBoastsMade_LocalSourceRange)
            std::byte localSourceStorage[sizeof(CGameScriptInterface_MsgOnBoastsMade_LocalSourceRange)];
        auto* const localSource =
            reinterpret_cast<CGameScriptInterface_MsgOnBoastsMade_LocalSourceRange*>(localSourceStorage);

        localSource->boastId = extraData->boastId;
        ::new (static_cast<void*>(std::addressof(localSource->boastText))) CCharString(extraData->boastText);

        MsgOnBoastsMade_Pair* const finish = outOverlay.last;
        if (finish == outOverlay.end)
        {
            std__vector_InsertRangeWithCopy(
                finish,
                std::addressof(localSource->boastId),
                std::addressof(localSource->sentinel),
                1,
                1);
        }
        else
        {
            if (finish != nullptr)
            {
                finish->first = localSource->boastId;
                ::new (static_cast<void*>(std::addressof(finish->second))) CCharString(localSource->boastText);
            }

            outOverlay.last = reinterpret_cast<MsgOnBoastsMade_Pair*>(
                reinterpret_cast<std::byte*>(outOverlay.last) + 0x08);
        }

        localSource->boastText.~CCharString();
    }

    auto* node = listHead->next;
    if (node != listHead)
    {
        int count = 0;
        auto* scan = node;
        do
        {
            scan = scan->next;
            ++count;
        } while (scan != listHead);

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
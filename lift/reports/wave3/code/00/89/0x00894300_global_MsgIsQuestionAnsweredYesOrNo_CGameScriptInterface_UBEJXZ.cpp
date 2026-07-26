#include <cstddef>

class CWorld;
class CMessageEvent;

class CMessageEventFilter_IsOfType
{
public:
    long Type;
};

class CMessageEventFilter_CanBeSeenOrHeard
{
public:
    long MaxWorldFrame;
};

template <class TFilter0, class TFilter1>
struct CFilter_Bind2
{
    TFilter0* pFilter0;
    TFilter1* pFilter1;
};

static_assert(
    offsetof(
        CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard>,
        pFilter0) == 0x00);
static_assert(
    offsetof(
        CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard>,
        pFilter1) == 0x04);

class CMessageEventManager
{
public:
    template <class TFilter>
    CMessageEvent* __thiscall FindMostRecentMessage(TFilter* pFilter);
};

class CGameScriptInterface
{
public:
    long __thiscall GetMaxWorldFrameForMessages() const;
    long __thiscall MsgIsQuestionAnsweredYesOrNo() const;
};

namespace
{
struct CGameScriptInterfaceWorldOverlay
{
    std::byte pad_00[0x04];
    CWorld* pWorld; // 0x04
};

static_assert(offsetof(CGameScriptInterfaceWorldOverlay, pWorld) == 0x04);

struct CWorldMessageEventManagerOverlay
{
    std::byte pad_00[0x60];
    CMessageEventManager* pMessageEventManager; // 0x60
};

static_assert(offsetof(CWorldMessageEventManagerOverlay, pMessageEventManager) == 0x60);

struct CMessageEventQuestionAnswerOverlay
{
    std::byte pad_00[0x3C];
    long Answer; // 0x3C
};

static_assert(offsetof(CMessageEventQuestionAnswerOverlay, Answer) == 0x3C);
}

long __thiscall CGameScriptInterface::MsgIsQuestionAnsweredYesOrNo() const
{
    const long maxWorldFrame = GetMaxWorldFrameForMessages();
    const long maxWorldFrameUnused = GetMaxWorldFrameForMessages();
    (void)maxWorldFrameUnused;

    CMessageEventFilter_IsOfType isOfTypeFilter;
    CMessageEventFilter_CanBeSeenOrHeard canBeSeenOrHeardFilter;
    CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> filter;

    filter.pFilter0 = &isOfTypeFilter;
    filter.pFilter1 = &canBeSeenOrHeardFilter;
    isOfTypeFilter.Type = 0x10;
    canBeSeenOrHeardFilter.MaxWorldFrame = maxWorldFrame;

    const auto* const pInterfaceOverlay =
        reinterpret_cast<const CGameScriptInterfaceWorldOverlay*>(this);
    const auto* const pWorldOverlay =
        reinterpret_cast<const CWorldMessageEventManagerOverlay*>(pInterfaceOverlay->pWorld);

    CMessageEvent* const pMessageEvent =
        pWorldOverlay->pMessageEventManager->FindMostRecentMessage(&filter);
    if (pMessageEvent != nullptr) {
        const long answer =
            reinterpret_cast<const CMessageEventQuestionAnswerOverlay*>(pMessageEvent)->Answer;
        if (answer == 1) {
            return answer;
        }
        if (answer == 0) {
            return answer;
        }
        if (answer == 2) {
            return answer;
        }
    }

    return -1;
}
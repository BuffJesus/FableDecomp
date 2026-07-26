#include <cstddef>
#include <cstdint>

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

static_assert(offsetof(CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard>, pFilter0) == 0x00);
static_assert(offsetof(CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard>, pFilter1) == 0x04);

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
    bool __thiscall MsgOnGameSavedManually() const;
};

namespace
{
struct CGameScriptInterfaceWorldOverlay
{
    std::byte pad_00[0x04];
    CWorld* pWorld;
};

static_assert(offsetof(CGameScriptInterfaceWorldOverlay, pWorld) == 0x04);

struct CWorldMessageEventManagerOverlay
{
    std::byte pad_00[0x60];
    CMessageEventManager* pMessageEventManager;
};

static_assert(offsetof(CWorldMessageEventManagerOverlay, pMessageEventManager) == 0x60);
}

bool __thiscall CGameScriptInterface::MsgOnGameSavedManually() const
{
    const long maxWorldFrame = GetMaxWorldFrameForMessages();
    const long maxWorldFrameUnused = GetMaxWorldFrameForMessages();
    (void)maxWorldFrameUnused;

    CMessageEventFilter_IsOfType isOfTypeFilter;
    CMessageEventFilter_CanBeSeenOrHeard canBeSeenOrHeardFilter;
    CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> filter;

    filter.pFilter0 = &isOfTypeFilter;
    filter.pFilter1 = &canBeSeenOrHeardFilter;
    isOfTypeFilter.Type = 0x3E;
    canBeSeenOrHeardFilter.MaxWorldFrame = maxWorldFrame;

    const auto* const pInterfaceOverlay =
        reinterpret_cast<const CGameScriptInterfaceWorldOverlay*>(this);
    const auto* const pWorldOverlay =
        reinterpret_cast<const CWorldMessageEventManagerOverlay*>(pInterfaceOverlay->pWorld);

    CMessageEvent* const pMessageEvent =
        pWorldOverlay->pMessageEventManager->FindMostRecentMessage(&filter);
    return pMessageEvent != nullptr;
}
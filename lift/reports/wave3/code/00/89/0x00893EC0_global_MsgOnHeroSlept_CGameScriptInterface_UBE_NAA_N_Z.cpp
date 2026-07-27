#include <cstddef>
#include <cstdint>

struct CMessageEvent;
struct CMessageEventManager;
struct CGameTimeManager;

struct CMessageEventFilter_IsOfType {
    std::uint32_t messageType;
};

struct CMessageEventFilter_CanBeSeenOrHeard {
    long maxWorldFrame0;
    long maxWorldFrame1;
};

template <typename TFilter0, typename TFilter1>
struct CFilter_Bind2 {
    TFilter0* filter0;
    TFilter1* filter1;
};

struct CGameScriptInterface_OwnerOverlay {
    std::byte pad_00[0x1C];
    CGameTimeManager* pGameTimeManager;
    std::byte pad_20[0x40];
    CMessageEventManager* pMessageEventManager;
};
static_assert(offsetof(CGameScriptInterface_OwnerOverlay, pGameTimeManager) == 0x1C);
static_assert(offsetof(CGameScriptInterface_OwnerOverlay, pMessageEventManager) == 0x60);

struct CGameScriptInterface_ThisOverlay {
    std::byte pad_00[0x04];
    CGameScriptInterface_OwnerOverlay* pOwner;
};
static_assert(offsetof(CGameScriptInterface_ThisOverlay, pOwner) == 0x04);

bool CGameScriptInterface::MsgOnHeroSlept(bool& outIsDay) const
{
    const long maxWorldFrame0 = CGameScriptInterface::GetMaxWorldFrameForMessages();
    CMessageEventFilter_CanBeSeenOrHeard canBeSeenOrHeardFilter{
        maxWorldFrame0,
        CGameScriptInterface::GetMaxWorldFrameForMessages(),
    };
    CMessageEventFilter_IsOfType isOfTypeFilter{0x42};
    CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> filter{
        &isOfTypeFilter,
        &canBeSeenOrHeardFilter,
    };

    auto* const pOwner = reinterpret_cast<const CGameScriptInterface_ThisOverlay*>(this)->pOwner;
    CMessageEvent* const pMessage =
        CMessageEventManager::FindMostRecentMessage<
            CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard>>(
                pOwner->pMessageEventManager,
                &filter);

    if (pMessage != nullptr) {
        outIsDay = CGameTimeManager::IsDay(pOwner->pGameTimeManager);
        return true;
    }

    return false;
}
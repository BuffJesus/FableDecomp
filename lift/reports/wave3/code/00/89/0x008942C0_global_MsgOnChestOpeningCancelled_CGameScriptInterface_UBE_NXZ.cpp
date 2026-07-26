#include <cstddef>
#include <cstdint>

class CWorld;
class CMessageEvent;

class CMessageEventFilter_IsOfType
{
public:
    long Type;
};

class CMessageEventManager
{
public:
    template <class TFilter>
    CMessageEvent* __thiscall FindMostRecentMessage(TFilter* pFilter);
};

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void __thiscall GetElementByIndex(long index);
};
}

class CGameScriptInterface
{
public:
    long __thiscall GetMaxWorldFrameForMessages() const;
    bool __thiscall MsgOnChestOpeningCancelled() const;
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

bool __thiscall CGameScriptInterface::MsgOnChestOpeningCancelled() const
{
    GetMaxWorldFrameForMessages();
    GetMaxWorldFrameForMessages();

    const auto* const pInterfaceOverlay =
        reinterpret_cast<const CGameScriptInterfaceWorldOverlay*>(this);
    const auto* const pWorldOverlay =
        reinterpret_cast<const CWorldMessageEventManagerOverlay*>(pInterfaceOverlay->pWorld);

    CMessageEventFilter_IsOfType isOfTypeFilter;
    isOfTypeFilter.Type = 0x48;

    CMessageEvent* const pMessageEvent =
        pWorldOverlay->pMessageEventManager->FindMostRecentMessage(&isOfTypeFilter);
    if (pMessageEvent != nullptr) {
        reinterpret_cast<NPlayerGui::CDrawQuestInfo*>(pWorldOverlay->pMessageEventManager)
            ->GetElementByIndex(static_cast<long>(reinterpret_cast<std::uintptr_t>(pMessageEvent)));
        return true;
    }

    return false;
}
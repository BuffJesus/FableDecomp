#include <cstddef>
#include <cstdint>

class CWorld;
class CCodeSectionManager;
class CMessageEvent;

extern std::uint8_t DAT_01375740;

template <typename T>
class CTBaseSingleton
{
public:
    static T* __cdecl Get();
};

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
    bool __thiscall MsgIsTutorialClickedPast() const;
};

namespace
{
struct CCodeSectionManagerTutorialOverlay
{
    std::byte pad_00[0x0C];
    std::uint8_t field_0C;
    std::byte pad_0D[0x17 - 0x0D];
    std::uint8_t field_17;
    std::byte pad_18[0xD7 - 0x18];
    std::uint8_t field_D7;
};

static_assert(offsetof(CCodeSectionManagerTutorialOverlay, field_0C) == 0x0C);
static_assert(offsetof(CCodeSectionManagerTutorialOverlay, field_17) == 0x17);
static_assert(offsetof(CCodeSectionManagerTutorialOverlay, field_D7) == 0xD7);

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

bool __thiscall CGameScriptInterface::MsgIsTutorialClickedPast() const
{
    if (DAT_01375740 != 0) {
        const auto* const pCodeSectionManager =
            reinterpret_cast<const CCodeSectionManagerTutorialOverlay*>(
                CTBaseSingleton<CCodeSectionManager>::Get());

        const std::uint8_t tutorialEnabled =
            (pCodeSectionManager->field_0C == 0)
                ? pCodeSectionManager->field_17
                : pCodeSectionManager->field_D7;

        if (tutorialEnabled != 0) {
            const long maxWorldFrame = GetMaxWorldFrameForMessages();
            const long maxWorldFrameUnused = GetMaxWorldFrameForMessages();
            (void)maxWorldFrameUnused;

            CMessageEventFilter_IsOfType isOfTypeFilter;
            CMessageEventFilter_CanBeSeenOrHeard canBeSeenOrHeardFilter;
            CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> filter;

            filter.pFilter0 = &isOfTypeFilter;
            filter.pFilter1 = &canBeSeenOrHeardFilter;
            isOfTypeFilter.Type = 0x12;
            canBeSeenOrHeardFilter.MaxWorldFrame = maxWorldFrame;

            const auto* const pInterfaceOverlay =
                reinterpret_cast<const CGameScriptInterfaceWorldOverlay*>(this);
            const auto* const pWorldOverlay =
                reinterpret_cast<const CWorldMessageEventManagerOverlay*>(pInterfaceOverlay->pWorld);

            CMessageEvent* const pMessageEvent =
                pWorldOverlay->pMessageEventManager->FindMostRecentMessage(&filter);
            return pMessageEvent != nullptr;
        }
    }

    return true;
}
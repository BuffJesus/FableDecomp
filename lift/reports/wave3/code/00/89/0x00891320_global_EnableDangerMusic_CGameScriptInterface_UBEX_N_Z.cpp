#include <cstddef>
#include <cstdint>

class CGameEvent
{
public:
    static void __thiscall AddUBYTE(CGameEvent* event, std::uint8_t value);
};

class CGameScriptInterface
{
public:
    void __thiscall EnableDangerMusic(bool enabled) const;
};

namespace
{
struct EnableDangerMusicInnerOverlay
{
    std::byte pad00[0x6C];
    CGameEvent* event_6C; // 0x6C
};

static_assert(offsetof(EnableDangerMusicInnerOverlay, event_6C) == 0x6C);

struct CGameScriptInterfaceOverlay
{
    std::byte pad00[0x04];
    EnableDangerMusicInnerOverlay* ptr_04; // 0x04
};

static_assert(offsetof(CGameScriptInterfaceOverlay, ptr_04) == 0x04);
}

void __thiscall CGameScriptInterface::EnableDangerMusic(bool enabled) const
{
    CGameEvent::AddUBYTE(
        reinterpret_cast<CGameScriptInterfaceOverlay const*>(this)->ptr_04->event_6C,
        static_cast<std::uint8_t>(enabled));
}
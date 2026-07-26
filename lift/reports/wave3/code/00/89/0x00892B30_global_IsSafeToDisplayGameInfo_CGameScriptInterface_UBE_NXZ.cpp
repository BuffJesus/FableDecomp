#include <cstddef>
#include <cstdint>

class CSubtitleRenderer
{
public:
    bool __thiscall IsAllowedToSetText(bool allow);
};

class CGameScriptInterface
{
public:
    bool __thiscall IsSafeToDisplayGameInfo() const;
};

namespace
{
struct IsSafeToDisplayGameInfoGlobalOverlay
{
    std::byte pad_00[0x18];
    std::byte subtitle_renderer_storage[0x1];
};

static_assert(
    offsetof(
        IsSafeToDisplayGameInfoGlobalOverlay,
        subtitle_renderer_storage) == 0x18);

extern "C" std::uint32_t DAT_013b8790;
}

bool __thiscall CGameScriptInterface::IsSafeToDisplayGameInfo() const
{
    auto* const global = reinterpret_cast<IsSafeToDisplayGameInfoGlobalOverlay*>(DAT_013b8790);
    auto* const subtitle_renderer =
        reinterpret_cast<CSubtitleRenderer*>(global->subtitle_renderer_storage);

    return subtitle_renderer->IsAllowedToSetText(false);
}
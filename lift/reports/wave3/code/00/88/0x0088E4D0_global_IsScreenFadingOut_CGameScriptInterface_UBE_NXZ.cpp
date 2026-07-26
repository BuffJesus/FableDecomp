#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    bool __thiscall IsScreenFadingOut() const;
};

namespace
{
struct ScreenFadeStateOverlay
{
    std::byte pad_00[0xBC];
    std::uint8_t field_BC;
    std::byte pad_BD[0xC];
    std::uint8_t field_C9;
};

static_assert(offsetof(ScreenFadeStateOverlay, field_BC) == 0xBC);
static_assert(offsetof(ScreenFadeStateOverlay, field_C9) == 0xC9);

struct CGameScriptInterfaceScreenFadeOverlay
{
    std::byte pad_00[0xC];
    std::uint32_t field_C;
};

static_assert(offsetof(CGameScriptInterfaceScreenFadeOverlay, field_C) == 0xC);
}

bool __thiscall CGameScriptInterface::IsScreenFadingOut() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterfaceScreenFadeOverlay*>(this);
    const auto* const state =
        reinterpret_cast<const ScreenFadeStateOverlay*>(self->field_C);

    if ((state->field_BC != 0) && (state->field_C9 != 0)) {
        return true;
    }

    return false;
}
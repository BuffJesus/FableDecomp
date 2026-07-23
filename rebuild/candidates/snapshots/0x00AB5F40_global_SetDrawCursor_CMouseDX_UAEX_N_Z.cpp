#include <cstddef>
#include <cstdint>

struct CMouseDX_SetDrawCursor_Overlay {
    std::byte pad_0000[0x343F];
    std::uint8_t draw_cursor;
};

static_assert(offsetof(CMouseDX_SetDrawCursor_Overlay, draw_cursor) == 0x343F);

void CMouseDX::SetDrawCursor(bool draw_cursor)
{
    auto* self = reinterpret_cast<CMouseDX_SetDrawCursor_Overlay*>(this);
    self->draw_cursor = static_cast<std::uint8_t>(draw_cursor);
}
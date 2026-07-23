#include <cstddef>

struct CMouseDX_IsDrawingCursor_Overlay {
    std::byte pad_0000[0x343F];
    bool draw_cursor;
};

static_assert(offsetof(CMouseDX_IsDrawingCursor_Overlay, draw_cursor) == 0x343F);

bool CMouseDX::IsDrawingCursor() const
{
    auto const* self = reinterpret_cast<CMouseDX_IsDrawingCursor_Overlay const*>(this);
    return self->draw_cursor;
}
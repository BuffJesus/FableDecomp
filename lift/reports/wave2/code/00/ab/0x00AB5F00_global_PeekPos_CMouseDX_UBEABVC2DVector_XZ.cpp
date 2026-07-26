#include <cstddef>

struct CMouseDX_PeekPos_Overlay {
    std::byte pad_0000[0x3414];
    C2DVector pos;
};

static_assert(offsetof(CMouseDX_PeekPos_Overlay, pos) == 0x3414);

const C2DVector& CMouseDX::PeekPos() const
{
    const auto* self = reinterpret_cast<const CMouseDX_PeekPos_Overlay*>(this);
    return self->pos;
}
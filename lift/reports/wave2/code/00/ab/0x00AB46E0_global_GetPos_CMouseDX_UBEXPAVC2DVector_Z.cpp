#include <cstddef>
#include <cstdint>

struct C2DVector_Overlay {
    std::uint32_t x;
    std::uint32_t y;
};

struct CMouseDX_GetPos_Overlay {
    std::byte pad_0000[0x3414];
    std::uint32_t pos_x;
    std::uint32_t pos_y;
};

static_assert(offsetof(CMouseDX_GetPos_Overlay, pos_x) == 0x3414);
static_assert(offsetof(CMouseDX_GetPos_Overlay, pos_y) == 0x3418);
static_assert(sizeof(C2DVector_Overlay) == 0x8);

void CMouseDX::GetPos(C2DVector* outPos)
{
    auto* self = reinterpret_cast<CMouseDX_GetPos_Overlay*>(this);
    auto* out = reinterpret_cast<C2DVector_Overlay*>(outPos);

    out->x = self->pos_x;
    out->y = self->pos_y;
}
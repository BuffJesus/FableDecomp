#include <cstddef>
#include <cstdint>

struct CMouseDX_SetInputOn_Overlay {
    std::byte pad_0000[0x343D];
    std::uint8_t input_on;
};

static_assert(offsetof(CMouseDX_SetInputOn_Overlay, input_on) == 0x343D);

void CMouseDX::SetInputOn(bool input_on)
{
    auto* self = reinterpret_cast<CMouseDX_SetInputOn_Overlay*>(this);
    self->input_on = static_cast<std::uint8_t>(input_on);
}
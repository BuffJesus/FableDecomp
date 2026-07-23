#include <cstddef>
#include <cstdint>

struct CMouseDX_SetMouseFiltering_Overlay {
    std::byte pad_0000[0x484B];
    std::uint8_t mouse_filtering;
};

static_assert(offsetof(CMouseDX_SetMouseFiltering_Overlay, mouse_filtering) == 0x484B);

void CMouseDX::SetMouseFiltering(bool mouse_filtering)
{
    auto* self = reinterpret_cast<CMouseDX_SetMouseFiltering_Overlay*>(this);
    self->mouse_filtering = static_cast<std::uint8_t>(mouse_filtering);
}
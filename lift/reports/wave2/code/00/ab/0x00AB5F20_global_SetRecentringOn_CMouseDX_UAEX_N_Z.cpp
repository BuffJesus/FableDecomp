#include <cstddef>
#include <cstdint>

struct CMouseDX_SetRecentringOn_Overlay {
    std::byte pad_0000[0x343E];
    std::uint8_t recentring_on;
};

static_assert(offsetof(CMouseDX_SetRecentringOn_Overlay, recentring_on) == 0x343E);

void CMouseDX::SetRecentringOn(bool recentring_on)
{
    auto* self = reinterpret_cast<CMouseDX_SetRecentringOn_Overlay*>(this);
    self->recentring_on = static_cast<std::uint8_t>(recentring_on);
}
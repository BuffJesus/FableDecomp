#include <cstddef>

struct CMouseDX_IsRecentringOn_Overlay {
    std::byte pad_0000[0x343E];
    bool recentring_on;
};

static_assert(offsetof(CMouseDX_IsRecentringOn_Overlay, recentring_on) == 0x343E);

bool CMouseDX::IsRecentringOn() const
{
    auto const* self = reinterpret_cast<CMouseDX_IsRecentringOn_Overlay const*>(this);
    return self->recentring_on;
}
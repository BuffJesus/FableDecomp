#include <cstddef>

struct CMouseDX_IsMMBDown_Overlay {
    std::byte pad_0000[0x4849];
    bool field_4849;
};

static_assert(offsetof(CMouseDX_IsMMBDown_Overlay, field_4849) == 0x4849);

bool CMouseDX::IsMMBDown() const
{
    auto const* self = reinterpret_cast<CMouseDX_IsMMBDown_Overlay const*>(this);
    return self->field_4849;
}
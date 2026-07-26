#include <cstddef>

struct CMouseDX_IsRMBDown_Overlay {
    std::byte pad_0000[0x484A];
    bool field_484A;
};

static_assert(offsetof(CMouseDX_IsRMBDown_Overlay, field_484A) == 0x484A);

bool CMouseDX::IsRMBDown() const
{
    auto const* self = reinterpret_cast<CMouseDX_IsRMBDown_Overlay const*>(this);
    return self->field_484A;
}
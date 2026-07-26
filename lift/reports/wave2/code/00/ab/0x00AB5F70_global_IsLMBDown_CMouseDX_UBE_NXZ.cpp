#include <cstddef>

struct CMouseDX_IsLMBDown_Overlay {
    std::byte pad_0000[0x4848];
    bool field_4848;
};

static_assert(offsetof(CMouseDX_IsLMBDown_Overlay, field_4848) == 0x4848);

bool CMouseDX::IsLMBDown() const
{
    auto const* self = reinterpret_cast<CMouseDX_IsLMBDown_Overlay const*>(this);
    return self->field_4848;
}
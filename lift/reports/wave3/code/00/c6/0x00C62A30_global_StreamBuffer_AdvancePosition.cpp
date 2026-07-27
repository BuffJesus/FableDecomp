#include <cstddef>
#include <cstdint>

struct StreamBuffer_Overlay {
    std::int32_t field_0x0;
    std::int32_t field_0x4;
};

static_assert(offsetof(StreamBuffer_Overlay, field_0x0) == 0x0);
static_assert(offsetof(StreamBuffer_Overlay, field_0x4) == 0x4);

void __thiscall StreamBuffer_AdvancePosition(StreamBuffer_Overlay* self, int count)
{
    self->field_0x0 = self->field_0x0 + self->field_0x4 * count * 2;
}
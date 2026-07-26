#include <cstddef>
#include <cstdint>

namespace
{
struct IsStateStreaming_2to4_Overlay
{
    std::uint8_t pad00[0xAC];
    std::int16_t fieldAC; // +0xAC
};

static_assert(offsetof(IsStateStreaming_2to4_Overlay, fieldAC) == 0xAC);
}

void __thiscall EmptyStub(void* self, std::int32_t value);

std::uint32_t __thiscall IsStateStreaming_2to4(IsStateStreaming_2to4_Overlay* self)
{
    EmptyStub(self, -1);

    const std::int16_t state = self->fieldAC;
    if (((state != 2) && (state != 3)) && (state != 4))
    {
        return 0;
    }

    return 1;
}
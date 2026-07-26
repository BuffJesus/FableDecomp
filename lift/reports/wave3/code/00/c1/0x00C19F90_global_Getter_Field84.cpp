#include <cstddef>
#include <cstdint>

namespace
{
struct Getter_Field84_Overlay
{
    std::uint8_t pad00[0x84];
    std::uint32_t field84; // +0x84
};

static_assert(offsetof(Getter_Field84_Overlay, field84) == 0x84);
}

std::uint32_t __fastcall Getter_Field84(Getter_Field84_Overlay* self)
{
    return self->field84;
}
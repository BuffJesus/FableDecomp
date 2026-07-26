#include <cstddef>
#include <cstdint>

namespace
{
struct Handle_InitInvalid_HandleOverlay
{
    std::int32_t field00;   // +0x00
    std::uint32_t field04;  // +0x04
    std::uint8_t field08;   // +0x08
    std::uint8_t pad09[3];  // +0x09
};
static_assert(sizeof(Handle_InitInvalid_HandleOverlay) == 0x0C);
static_assert(offsetof(Handle_InitInvalid_HandleOverlay, field00) == 0x00);
static_assert(offsetof(Handle_InitInvalid_HandleOverlay, field04) == 0x04);
static_assert(offsetof(Handle_InitInvalid_HandleOverlay, field08) == 0x08);
}

void __fastcall Handle_InitInvalid_008702d0(Handle_InitInvalid_HandleOverlay* handle)
{
    handle->field00 = -1;
    handle->field04 = 0;
    handle->field08 = 0;
}
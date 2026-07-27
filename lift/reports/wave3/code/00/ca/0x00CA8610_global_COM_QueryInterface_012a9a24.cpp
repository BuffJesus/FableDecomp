#include <cstddef>
#include <cstdint>

struct COM_QueryInterface_012a9a24_This
{
    std::byte pad_00[0x1C];
    std::uint32_t value_at_1C;
    std::byte pad_20[0x7C];
    std::uint32_t value_at_9C;
};

static_assert(offsetof(COM_QueryInterface_012a9a24_This, value_at_1C) == 0x1C);
static_assert(offsetof(COM_QueryInterface_012a9a24_This, value_at_9C) == 0x9C);

using RawMethodAt24 = void(__stdcall*)(void* /*param_2*/, std::uint32_t* /*inout_identity*/);
using RawQueryInterface = std::uint32_t(__stdcall*)(void* /*param_2*/, const void* /*iid*/, void* /*out*/);

std::uint32_t __thiscall COM_QueryInterface_012a9a24(
    COM_QueryInterface_012a9a24_This* self,
    void* param_2)
{
    std::uint32_t identity = reinterpret_cast<std::uint32_t>(param_2);

    auto* vtable = *reinterpret_cast<void***>(param_2);
    reinterpret_cast<RawMethodAt24>(vtable[9])(param_2, &identity);

    if (identity == self->value_at_1C) {
        return 0x80040208u;
    }

    vtable = *reinterpret_cast<void***>(param_2);
    const std::uint32_t result =
        reinterpret_cast<RawQueryInterface>(vtable[0])(
            param_2,
            reinterpret_cast<const void*>(0x012A9A24),
            &self->value_at_9C);

    return result & (static_cast<std::uint32_t>(static_cast<std::int32_t>(result) >= 0) - 1u);
}
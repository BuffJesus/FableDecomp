#include <cstddef>
#include <cstdint>

struct Script_GetEntryPointer_Overlay
{
    std::uint8_t pad_00[0x04];
    std::int32_t field_04;
    std::uint8_t pad_08[0x2C];
    std::int32_t field_34;
    std::uint8_t pad_38[0x14];
    std::int32_t field_4C;
    std::uint8_t pad_50[0x08];
    std::int32_t field_58;
    std::uint8_t pad_5C[0x04];
    std::int32_t field_60;
};

static_assert(offsetof(Script_GetEntryPointer_Overlay, field_04) == 0x04);
static_assert(offsetof(Script_GetEntryPointer_Overlay, field_34) == 0x34);
static_assert(offsetof(Script_GetEntryPointer_Overlay, field_4C) == 0x4C);
static_assert(offsetof(Script_GetEntryPointer_Overlay, field_58) == 0x58);
static_assert(offsetof(Script_GetEntryPointer_Overlay, field_60) == 0x60);

std::int32_t __fastcall Script_GetEntryPointer(void* script, std::int32_t index)
{
    const auto* const self = static_cast<const Script_GetEntryPointer_Overlay*>(script);

    if (self->field_04 != 0)
    {
        if (index < 0)
        {
            if (self->field_58 < 3)
            {
                return self->field_4C;
            }

            index = self->field_60;
        }
        else if (self->field_34 <= index)
        {
            return 0;
        }

        return index * 0x10 + self->field_4C;
    }

    return self->field_4C;
}
#include <cstddef>
#include <cstdint>

namespace
{
struct CTCInventory_Overlay
{
    std::uint8_t pad00[0x0E];
    std::int16_t slotType; // +0x0E
};

static_assert(offsetof(CTCInventory_Overlay, slotType) == 0x0E);
}

std::int32_t __fastcall CTCInventory_CheckSlotTypeNegative(CTCInventory_Overlay* self)
{
    const std::int16_t slotType = self->slotType;
    if ((slotType != 0) && ((slotType == 1) || (slotType == 2)))
    {
        return -1;
    }

    return 0;
}
#include <cstddef>
#include <cstdint>

struct CPlayerManager_00449970_Overlay {
    std::byte pad_00[0x1C];
    std::int32_t field_1C;
};
static_assert(offsetof(CPlayerManager_00449970_Overlay, field_1C) == 0x1C);

void __fastcall Catch_0040d959(CPlayerManager* const self)
{
    const auto* const overlay =
        reinterpret_cast<const CPlayerManager_00449970_Overlay*>(self);

    (void)self->GetPlayer(overlay->field_1C);
}
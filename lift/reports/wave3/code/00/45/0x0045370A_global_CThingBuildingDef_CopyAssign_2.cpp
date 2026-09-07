#include <cstddef>
#include <cstdint>

#pragma pack(push, 1)
struct CThingBuildingDef_CopyAssign_2_Overlay {
    std::byte pad_00[0x25];
    std::uint32_t field_25;
    std::uint32_t field_29;
    std::uint32_t field_2D;
    std::uint16_t field_31;
    std::uint32_t field_34;
    std::uint32_t field_38;
    std::uint32_t field_3C;
    std::uint32_t field_40;
    std::uint32_t field_44;
    std::uint32_t field_48;
    std::uint32_t field_4C;
    std::uint32_t field_50;
    std::uint32_t field_54;
    std::uint32_t field_58;
    std::uint8_t field_5C;
    std::uint8_t field_5D;
    std::uint8_t field_5E;
    std::uint8_t field_5F;
    std::uint8_t field_60;
    std::uint8_t field_61;
    std::byte pad_62[0x02];
    std::uint32_t field_64;
    std::uint32_t field_68;
    std::uint32_t field_6C;
};
#pragma pack(pop)

static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_25) == 0x25);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_29) == 0x29);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_2D) == 0x2D);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_31) == 0x31);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_34) == 0x34);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_38) == 0x38);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_3C) == 0x3C);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_40) == 0x40);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_44) == 0x44);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_48) == 0x48);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_4C) == 0x4C);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_50) == 0x50);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_54) == 0x54);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_58) == 0x58);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_5C) == 0x5C);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_5D) == 0x5D);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_5E) == 0x5E);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_5F) == 0x5F);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_60) == 0x60);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_61) == 0x61);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_64) == 0x64);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_68) == 0x68);
static_assert(offsetof(CThingBuildingDef_CopyAssign_2_Overlay, field_6C) == 0x6C);

CThingBuildingDef* __thiscall CThingBuildingDef_CopyAssign_2(
    CThingBuildingDef* self,
    CThingBuildingDef* other)
{
    self->CThingBuildingDef::operator=(other);

    auto* dst = reinterpret_cast<CThingBuildingDef_CopyAssign_2_Overlay*>(self);
    const auto* src = reinterpret_cast<const CThingBuildingDef_CopyAssign_2_Overlay*>(other);

    dst->field_25 = src->field_25;
    dst->field_29 = src->field_29;
    dst->field_2D = src->field_2D;
    dst->field_31 = src->field_31;
    dst->field_34 = src->field_34;
    dst->field_38 = src->field_38;
    dst->field_3C = src->field_3C;
    dst->field_40 = src->field_40;
    dst->field_44 = src->field_44;
    dst->field_48 = src->field_48;
    dst->field_4C = src->field_4C;
    dst->field_50 = src->field_50;
    dst->field_54 = src->field_54;
    dst->field_58 = src->field_58;
    dst->field_5C = src->field_5C;
    dst->field_5D = src->field_5D;
    dst->field_5E = src->field_5E;
    dst->field_5F = src->field_5F;
    dst->field_60 = src->field_60;
    dst->field_61 = src->field_61;
    dst->field_64 = src->field_64;
    dst->field_68 = src->field_68;
    dst->field_6C = src->field_6C;

    return self;
}
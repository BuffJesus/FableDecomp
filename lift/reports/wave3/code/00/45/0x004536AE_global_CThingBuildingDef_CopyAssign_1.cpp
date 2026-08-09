struct CThingBuildingDef_CopyAssign_1_Overlay {
    std::byte pad_00[0x28];
    std::uint32_t field_28;
    std::uint32_t field_2C;
    std::uint32_t field_30;
    std::uint32_t field_34;
    std::uint32_t field_38;
    std::uint32_t field_3C;
    std::uint32_t field_40;
    std::uint32_t field_44;
    std::uint32_t field_48;
    std::uint8_t field_4C;
    std::byte pad_4D[0x03];
    std::uint32_t field_50;
};

static_assert(offsetof(CThingBuildingDef_CopyAssign_1_Overlay, field_28) == 0x28);
static_assert(offsetof(CThingBuildingDef_CopyAssign_1_Overlay, field_2C) == 0x2C);
static_assert(offsetof(CThingBuildingDef_CopyAssign_1_Overlay, field_30) == 0x30);
static_assert(offsetof(CThingBuildingDef_CopyAssign_1_Overlay, field_34) == 0x34);
static_assert(offsetof(CThingBuildingDef_CopyAssign_1_Overlay, field_38) == 0x38);
static_assert(offsetof(CThingBuildingDef_CopyAssign_1_Overlay, field_3C) == 0x3C);
static_assert(offsetof(CThingBuildingDef_CopyAssign_1_Overlay, field_40) == 0x40);
static_assert(offsetof(CThingBuildingDef_CopyAssign_1_Overlay, field_44) == 0x44);
static_assert(offsetof(CThingBuildingDef_CopyAssign_1_Overlay, field_48) == 0x48);
static_assert(offsetof(CThingBuildingDef_CopyAssign_1_Overlay, field_4C) == 0x4C);
static_assert(offsetof(CThingBuildingDef_CopyAssign_1_Overlay, field_50) == 0x50);

CThingBuildingDef* __thiscall CThingBuildingDef_CopyAssign_1(
    CThingBuildingDef* self,
    CThingBuildingDef* other)
{
    self->CThingBuildingDef::operator=(*other);

    auto* dst = reinterpret_cast<CThingBuildingDef_CopyAssign_1_Overlay*>(self);
    auto* src = reinterpret_cast<CThingBuildingDef_CopyAssign_1_Overlay*>(other);

    dst->field_28 = src->field_28;
    dst->field_2C = src->field_2C;
    dst->field_30 = src->field_30;
    dst->field_34 = src->field_34;
    dst->field_38 = src->field_38;
    dst->field_3C = src->field_3C;
    dst->field_40 = src->field_40;
    dst->field_44 = src->field_44;
    dst->field_48 = src->field_48;
    dst->field_4C = src->field_4C;
    dst->field_50 = src->field_50;

    return self;
}
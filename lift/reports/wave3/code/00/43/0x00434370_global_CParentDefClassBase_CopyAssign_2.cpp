struct CParentDefClassBase_CopyAssign_2_Overlay {
    std::byte pad_00[0x3C];
    std::uint32_t field_3C;
    std::uint32_t field_40;
    std::uint32_t field_44;
    std::uint32_t field_48;
    std::uint32_t field_4C;
    std::uint32_t field_50;
    std::uint32_t field_54;
    std::uint32_t field_58;
    std::uint32_t field_5C;
    std::uint32_t field_60;
    std::uint32_t field_64;
    std::uint32_t field_68;
    std::uint32_t field_6C;
    std::uint32_t field_70;
    std::uint32_t field_74;
    std::uint32_t field_78;
    std::uint32_t field_7C;
};

static_assert(offsetof(CParentDefClassBase_CopyAssign_2_Overlay, field_3C) == 0x3C);
static_assert(offsetof(CParentDefClassBase_CopyAssign_2_Overlay, field_40) == 0x40);
static_assert(offsetof(CParentDefClassBase_CopyAssign_2_Overlay, field_44) == 0x44);
static_assert(offsetof(CParentDefClassBase_CopyAssign_2_Overlay, field_48) == 0x48);
static_assert(offsetof(CParentDefClassBase_CopyAssign_2_Overlay, field_4C) == 0x4C);
static_assert(offsetof(CParentDefClassBase_CopyAssign_2_Overlay, field_50) == 0x50);
static_assert(offsetof(CParentDefClassBase_CopyAssign_2_Overlay, field_54) == 0x54);
static_assert(offsetof(CParentDefClassBase_CopyAssign_2_Overlay, field_58) == 0x58);
static_assert(offsetof(CParentDefClassBase_CopyAssign_2_Overlay, field_5C) == 0x5C);
static_assert(offsetof(CParentDefClassBase_CopyAssign_2_Overlay, field_60) == 0x60);
static_assert(offsetof(CParentDefClassBase_CopyAssign_2_Overlay, field_64) == 0x64);
static_assert(offsetof(CParentDefClassBase_CopyAssign_2_Overlay, field_68) == 0x68);
static_assert(offsetof(CParentDefClassBase_CopyAssign_2_Overlay, field_6C) == 0x6C);
static_assert(offsetof(CParentDefClassBase_CopyAssign_2_Overlay, field_70) == 0x70);
static_assert(offsetof(CParentDefClassBase_CopyAssign_2_Overlay, field_74) == 0x74);
static_assert(offsetof(CParentDefClassBase_CopyAssign_2_Overlay, field_78) == 0x78);
static_assert(offsetof(CParentDefClassBase_CopyAssign_2_Overlay, field_7C) == 0x7C);

CParentDefClassBase* __thiscall CParentDefClassBase_CopyAssign_2(
    CParentDefClassBase* self,
    CParentDefClassBase* other)
{
    self->CParentDefClassBase::operator=(*other);

    auto* dst = reinterpret_cast<CParentDefClassBase_CopyAssign_2_Overlay*>(self);
    auto* src = reinterpret_cast<CParentDefClassBase_CopyAssign_2_Overlay*>(other);

    dst->field_3C = src->field_3C;
    dst->field_40 = src->field_40;
    dst->field_44 = src->field_44;
    dst->field_48 = src->field_48;
    dst->field_4C = src->field_4C;
    dst->field_50 = src->field_50;
    dst->field_54 = src->field_54;
    dst->field_58 = src->field_58;
    dst->field_5C = src->field_5C;
    dst->field_60 = src->field_60;
    dst->field_64 = src->field_64;
    dst->field_68 = src->field_68;
    dst->field_6C = src->field_6C;
    dst->field_70 = src->field_70;
    dst->field_74 = src->field_74;
    dst->field_78 = src->field_78;
    dst->field_7C = src->field_7C;

    return self;
}
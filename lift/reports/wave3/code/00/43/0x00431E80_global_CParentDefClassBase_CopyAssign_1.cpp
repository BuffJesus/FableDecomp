struct CParentDefClassBase_CopyAssign_1_VectorOverlay {
    std::byte storage[0x0C];
};

struct CParentDefClassBase_CopyAssign_1_Overlay {
    std::byte pad_00[0x3C];
    CParentDefClassBase_CopyAssign_1_VectorOverlay field_3C;
    std::uint8_t field_48;
    std::uint8_t field_49;
    std::uint8_t field_4A;
    std::uint8_t field_4B;
    std::uint8_t field_4C;
    std::uint8_t field_4D;
};

static_assert(offsetof(CParentDefClassBase_CopyAssign_1_Overlay, field_3C) == 0x3C);
static_assert(offsetof(CParentDefClassBase_CopyAssign_1_Overlay, field_48) == 0x48);
static_assert(offsetof(CParentDefClassBase_CopyAssign_1_Overlay, field_49) == 0x49);
static_assert(offsetof(CParentDefClassBase_CopyAssign_1_Overlay, field_4A) == 0x4A);
static_assert(offsetof(CParentDefClassBase_CopyAssign_1_Overlay, field_4B) == 0x4B);
static_assert(offsetof(CParentDefClassBase_CopyAssign_1_Overlay, field_4C) == 0x4C);
static_assert(offsetof(CParentDefClassBase_CopyAssign_1_Overlay, field_4D) == 0x4D);

extern void __thiscall std_vector_assign(
    CParentDefClassBase_CopyAssign_1_VectorOverlay* self,
    CParentDefClassBase_CopyAssign_1_VectorOverlay* other);

CParentDefClassBase* __thiscall CParentDefClassBase_CopyAssign_1(
    CParentDefClassBase* self,
    CParentDefClassBase* other)
{
    self->CParentDefClassBase::operator=(*other);

    auto* dst = reinterpret_cast<CParentDefClassBase_CopyAssign_1_Overlay*>(self);
    auto* src = reinterpret_cast<CParentDefClassBase_CopyAssign_1_Overlay*>(other);

    std_vector_assign(&dst->field_3C, &src->field_3C);

    dst->field_48 = src->field_48;
    dst->field_49 = src->field_49;
    dst->field_4A = src->field_4A;
    dst->field_4B = src->field_4B;
    dst->field_4C = src->field_4C;
    dst->field_4D = src->field_4D;

    return self;
}
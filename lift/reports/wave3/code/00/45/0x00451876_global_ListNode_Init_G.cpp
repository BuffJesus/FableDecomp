#include <cstddef>
#include <cstdint>

#pragma pack(push, 1)

struct ListNode_G_Overlay {
    std::uint8_t field_0x00;
    std::uint8_t pad_0x01[3];
    std::uint32_t field_0x04;
    ListNode_G_Overlay* field_0x08;
    ListNode_G_Overlay* field_0x0C;
};

static_assert(offsetof(ListNode_G_Overlay, field_0x00) == 0x00);
static_assert(offsetof(ListNode_G_Overlay, field_0x04) == 0x04);
static_assert(offsetof(ListNode_G_Overlay, field_0x08) == 0x08);
static_assert(offsetof(ListNode_G_Overlay, field_0x0C) == 0x0C);

struct ListNode_Init_G_Overlay {
    ListNode_G_Overlay* field_0x00;
    std::uint32_t field_0x04;
};

static_assert(offsetof(ListNode_Init_G_Overlay, field_0x00) == 0x00);
static_assert(offsetof(ListNode_Init_G_Overlay, field_0x04) == 0x04);

#pragma pack(pop)

// Observed call shape preserves ECX=self and pushes param_3.
extern void __thiscall ListNode_AllocateNode_Variant_G(
    ListNode_Init_G_Overlay* self,
    std::uint32_t param_3);

ListNode_Init_G_Overlay* __thiscall ListNode_Init_G(
    ListNode_Init_G_Overlay* self,
    std::uint32_t /*param_2*/,
    std::uint32_t param_3)
{
    ListNode_AllocateNode_Variant_G(self, param_3);
    self->field_0x04 = 0;
    self->field_0x00->field_0x00 = 0;
    self->field_0x00->field_0x04 = 0;
    self->field_0x00->field_0x08 = self->field_0x00;
    self->field_0x00->field_0x0C = self->field_0x00;
    return self;
}
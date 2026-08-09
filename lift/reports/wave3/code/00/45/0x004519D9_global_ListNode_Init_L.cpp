#include <cstddef>
#include <cstdint>

static_assert(sizeof(void*) == 4);

struct ListNode_Link_Overlay {
    std::uint8_t field_0x00;
    std::uint8_t pad_0x01[3];
    std::uint32_t field_0x04;
    ListNode_Link_Overlay* field_0x08;
    ListNode_Link_Overlay* field_0x0C;
};

static_assert(offsetof(ListNode_Link_Overlay, field_0x00) == 0x00);
static_assert(offsetof(ListNode_Link_Overlay, field_0x04) == 0x04);
static_assert(offsetof(ListNode_Link_Overlay, field_0x08) == 0x08);
static_assert(offsetof(ListNode_Link_Overlay, field_0x0C) == 0x0C);

struct ListNode_Init_Overlay {
    ListNode_Link_Overlay* field_0x00;
    std::uint32_t field_0x04;
};

static_assert(offsetof(ListNode_Init_Overlay, field_0x00) == 0x00);
static_assert(offsetof(ListNode_Init_Overlay, field_0x04) == 0x04);

void __cdecl ListNode_AllocateNode_Variant_L(std::uint32_t param_3);

ListNode_Init_Overlay* __thiscall ListNode_Init_L(
    ListNode_Init_Overlay* self,
    std::uint32_t /*param_2*/,
    std::uint32_t param_3)
{
    ListNode_AllocateNode_Variant_L(param_3);
    self->field_0x04 = 0;
    self->field_0x00->field_0x00 = 0;
    self->field_0x00->field_0x04 = 0;
    self->field_0x00->field_0x08 = self->field_0x00;
    self->field_0x00->field_0x0C = self->field_0x00;
    return self;
}
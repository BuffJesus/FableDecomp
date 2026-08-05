#include <cstddef>
#include <cstdint>

struct ListNode_InnerOverlay {
    std::uint8_t state;
    std::uint8_t pad_01[3];
    std::uint32_t value_04;
    ListNode_InnerOverlay* next;
    ListNode_InnerOverlay* prev;
};

static_assert(offsetof(ListNode_InnerOverlay, state) == 0x00);
static_assert(offsetof(ListNode_InnerOverlay, value_04) == 0x04);
static_assert(offsetof(ListNode_InnerOverlay, next) == 0x08);
static_assert(offsetof(ListNode_InnerOverlay, prev) == 0x0C);

struct ListNode_Init_E_Overlay {
    ListNode_InnerOverlay* node;
    std::uint32_t value_04;
};

static_assert(offsetof(ListNode_Init_E_Overlay, node) == 0x00);
static_assert(offsetof(ListNode_Init_E_Overlay, value_04) == 0x04);

extern void ListNode_AllocateNode_Variant_E(std::uint32_t param_3);

ListNode_Init_E_Overlay* __thiscall ListNode_Init_E(
    ListNode_Init_E_Overlay* self,
    std::uint32_t /*param_2*/,
    std::uint32_t param_3)
{
    ListNode_AllocateNode_Variant_E(param_3);
    self->value_04 = 0;
    self->node->state = 0;
    self->node->value_04 = 0;
    self->node->next = self->node;
    self->node->prev = self->node;
    return self;
}
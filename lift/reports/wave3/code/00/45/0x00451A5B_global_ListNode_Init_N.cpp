#include <cstddef>
#include <cstdint>

struct ListNode_Init_N_NodeOverlay {
    std::uint8_t state;
    std::uint8_t pad_01[3];
    std::uint32_t value_at_04;
    ListNode_Init_N_NodeOverlay* next;
    ListNode_Init_N_NodeOverlay* prev;
};

static_assert(offsetof(ListNode_Init_N_NodeOverlay, state) == 0x00);
static_assert(offsetof(ListNode_Init_N_NodeOverlay, value_at_04) == 0x04);
static_assert(offsetof(ListNode_Init_N_NodeOverlay, next) == 0x08);
static_assert(offsetof(ListNode_Init_N_NodeOverlay, prev) == 0x0C);
static_assert(sizeof(ListNode_Init_N_NodeOverlay) == 0x10);

struct ListNode_Init_N_Overlay {
    ListNode_Init_N_NodeOverlay* node;
    std::uint32_t field_04;
};

static_assert(offsetof(ListNode_Init_N_Overlay, node) == 0x00);
static_assert(offsetof(ListNode_Init_N_Overlay, field_04) == 0x04);
static_assert(sizeof(ListNode_Init_N_Overlay) == 0x08);

void __stdcall ListNode_AllocateNode_Variant_N(std::uint32_t param_3);

ListNode_Init_N_Overlay* __thiscall ListNode_Init_N(
    ListNode_Init_N_Overlay* self,
    std::uint32_t /*param_2*/,
    std::uint32_t param_3)
{
    ListNode_AllocateNode_Variant_N(param_3);
    self->field_04 = 0;

    self->node->state = 0;
    self->node->value_at_04 = 0;
    self->node->next = self->node;
    self->node->prev = self->node;

    return self;
}
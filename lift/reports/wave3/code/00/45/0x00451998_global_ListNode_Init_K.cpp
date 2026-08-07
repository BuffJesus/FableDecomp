#include <cstddef>
#include <cstdint>

struct ListNode_Init_K_NodeOverlay {
    std::uint8_t state;
    std::uint8_t pad_01[3];
    std::uint32_t value_04;
    ListNode_Init_K_NodeOverlay* next;
    ListNode_Init_K_NodeOverlay* prev;
};

static_assert(offsetof(ListNode_Init_K_NodeOverlay, state) == 0x00);
static_assert(offsetof(ListNode_Init_K_NodeOverlay, value_04) == 0x04);
static_assert(offsetof(ListNode_Init_K_NodeOverlay, next) == 0x08);
static_assert(offsetof(ListNode_Init_K_NodeOverlay, prev) == 0x0C);

struct ListNode_Init_K_Overlay {
    ListNode_Init_K_NodeOverlay* node;
    std::uint32_t value_04;
};

static_assert(offsetof(ListNode_Init_K_Overlay, node) == 0x00);
static_assert(offsetof(ListNode_Init_K_Overlay, value_04) == 0x04);

extern "C" void __cdecl ListNode_AllocateNode_Variant_K(std::uint32_t param_3);

int* __thiscall ListNode_Init_K(ListNode_Init_K_Overlay* self, std::uint32_t /*param_2*/, std::uint32_t param_3)
{
    ListNode_AllocateNode_Variant_K(param_3);

    self->value_04 = 0;

    self->node->state = 0;
    self->node->value_04 = 0;
    self->node->next = self->node;
    self->node->prev = self->node;

    return reinterpret_cast<int*>(self);
}
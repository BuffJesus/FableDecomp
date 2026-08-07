#include <cstddef>
#include <cstdint>

struct ListNodeStorageOverlay {
    std::uint8_t state;
    std::uint8_t pad_01[3];
    std::uint32_t value_at_04;
    ListNodeStorageOverlay* next;
    ListNodeStorageOverlay* prev;
};

static_assert(offsetof(ListNodeStorageOverlay, state) == 0x00);
static_assert(offsetof(ListNodeStorageOverlay, value_at_04) == 0x04);
static_assert(offsetof(ListNodeStorageOverlay, next) == 0x08);
static_assert(offsetof(ListNodeStorageOverlay, prev) == 0x0C);

struct ListNodeInitOverlay {
    ListNodeStorageOverlay* node;
    std::uint32_t value_at_04;
};

static_assert(offsetof(ListNodeInitOverlay, node) == 0x00);
static_assert(offsetof(ListNodeInitOverlay, value_at_04) == 0x04);

extern "C" void __cdecl ListNode_AllocateNode_Variant_C(std::uint32_t param_3);

ListNodeInitOverlay* __thiscall ListNode_Init_C(
    ListNodeInitOverlay* self,
    std::uint32_t /*param_2*/,
    std::uint32_t param_3)
{
    ListNode_AllocateNode_Variant_C(param_3);
    self->value_at_04 = 0;

    self->node->state = 0;
    self->node->value_at_04 = 0;
    self->node->next = self->node;
    self->node->prev = self->node;

    return self;
}
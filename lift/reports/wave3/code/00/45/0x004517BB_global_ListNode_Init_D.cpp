#include <cstddef>
#include <cstdint>

struct ListNodeEntryOverlay_D {
    std::uint8_t state;
    std::uint8_t padding_01[3];
    void* value;
    ListNodeEntryOverlay_D* next;
    ListNodeEntryOverlay_D* prev;
};

static_assert(offsetof(ListNodeEntryOverlay_D, state) == 0x0);
static_assert(offsetof(ListNodeEntryOverlay_D, value) == 0x4);
static_assert(offsetof(ListNodeEntryOverlay_D, next) == 0x8);
static_assert(offsetof(ListNodeEntryOverlay_D, prev) == 0xC);

struct ListNodeInitOverlay_D {
    ListNodeEntryOverlay_D* node;
    std::uint32_t count;
};

static_assert(offsetof(ListNodeInitOverlay_D, node) == 0x0);
static_assert(offsetof(ListNodeInitOverlay_D, count) == 0x4);

extern "C" void __cdecl ListNode_AllocateNode_Variant_D(std::uint32_t param_3);

ListNodeInitOverlay_D* __thiscall ListNode_Init_D(
    ListNodeInitOverlay_D* self,
    std::uint32_t /*param_2*/,
    std::uint32_t param_3)
{
    ListNode_AllocateNode_Variant_D(param_3);
    self->count = 0;
    self->node->state = 0;
    self->node->value = nullptr;
    self->node->next = self->node;
    self->node->prev = self->node;
    return self;
}
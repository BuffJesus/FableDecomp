#include <cstddef>
#include <cstdint>

struct LinkedListNodeOverlay
{
    std::uint8_t field_0x0;
    std::uint8_t pad_0x1[3];
    std::uint32_t field_0x4;
    LinkedListNodeOverlay* next;
    LinkedListNodeOverlay* prev;
};

static_assert(offsetof(LinkedListNodeOverlay, field_0x0) == 0x0);
static_assert(offsetof(LinkedListNodeOverlay, field_0x4) == 0x4);
static_assert(offsetof(LinkedListNodeOverlay, next) == 0x8);
static_assert(offsetof(LinkedListNodeOverlay, prev) == 0xC);

struct LinkedListOverlay
{
    LinkedListNodeOverlay* node;
    std::uint32_t field_0x4;
};

static_assert(offsetof(LinkedListOverlay, node) == 0x0);
static_assert(offsetof(LinkedListOverlay, field_0x4) == 0x4);

void __stdcall LinkedList_Initialize(std::uint32_t param_3);

LinkedListOverlay* __thiscall LinkedList_InitializeNode(
    LinkedListOverlay* self,
    std::uint32_t /*param_2*/,
    std::uint32_t param_3)
{
    LinkedList_Initialize(param_3);
    self->field_0x4 = 0;
    self->node->field_0x0 = 0;
    self->node->field_0x4 = 0;
    self->node->next = self->node;
    self->node->prev = self->node;
    return self;
}
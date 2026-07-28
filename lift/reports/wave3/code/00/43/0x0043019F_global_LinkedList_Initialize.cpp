#include <cstddef>
#include <cstdint>

struct LinkedListNodeOverlay {
    std::uint8_t value0;
    std::uint8_t pad_01[3];
    std::uint32_t value4;
    LinkedListNodeOverlay* next;
    LinkedListNodeOverlay* prev;
};

static_assert(offsetof(LinkedListNodeOverlay, value0) == 0x0);
static_assert(offsetof(LinkedListNodeOverlay, value4) == 0x4);
static_assert(offsetof(LinkedListNodeOverlay, next) == 0x8);
static_assert(offsetof(LinkedListNodeOverlay, prev) == 0xC);

struct LinkedListOverlay {
    LinkedListNodeOverlay* head;
    std::int32_t count;
};

static_assert(offsetof(LinkedListOverlay, head) == 0x0);
static_assert(offsetof(LinkedListOverlay, count) == 0x4);

extern "C" void __stdcall Queue_Initialize(std::uint32_t param_3);

extern "C" LinkedListOverlay* __thiscall LinkedList_Initialize(
    LinkedListOverlay* self,
    std::uint32_t /*param_2*/,
    std::uint32_t param_3)
{
    Queue_Initialize(param_3);
    self->count = 0;
    self->head->value0 = 0;
    self->head->value4 = 0;
    self->head->next = self->head;
    self->head->prev = self->head;
    return self;
}
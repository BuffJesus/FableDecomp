#include <cstddef>
#include <cstdint>

struct ListNodeEntryOverlay {
    std::uint8_t state;
    std::uint8_t pad_0x01[3];
    void* payload;
    ListNodeEntryOverlay* next;
    ListNodeEntryOverlay* prev;
};

static_assert(offsetof(ListNodeEntryOverlay, state) == 0x00);
static_assert(offsetof(ListNodeEntryOverlay, payload) == 0x04);
static_assert(offsetof(ListNodeEntryOverlay, next) == 0x08);
static_assert(offsetof(ListNodeEntryOverlay, prev) == 0x0C);

struct ListNodeInitOverlay {
    ListNodeEntryOverlay* head;
    std::uint32_t count;
};

static_assert(offsetof(ListNodeInitOverlay, head) == 0x00);
static_assert(offsetof(ListNodeInitOverlay, count) == 0x04);

void ListNode_AllocateNode_Variant_M(std::uint32_t variant);

ListNodeInitOverlay* __thiscall ListNode_Init_M(
    ListNodeInitOverlay* self,
    std::uint32_t /*unused*/,
    std::uint32_t variant)
{
    ListNode_AllocateNode_Variant_M(variant);
    self->count = 0;
    self->head->state = 0;
    self->head->payload = nullptr;
    self->head->next = self->head;
    self->head->prev = self->head;
    return self;
}
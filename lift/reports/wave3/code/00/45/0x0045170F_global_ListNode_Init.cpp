#include <cstddef>
#include <cstdint>

struct ListNodeStorage
{
    std::uint8_t initializedFlag;   // +0x00
    std::uint8_t pad_01[3];         // +0x01
    std::uint32_t field_04;         // +0x04
    ListNodeStorage* next;          // +0x08
    ListNodeStorage* prev;          // +0x0C
};

static_assert(offsetof(ListNodeStorage, initializedFlag) == 0x00);
static_assert(offsetof(ListNodeStorage, field_04) == 0x04);
static_assert(offsetof(ListNodeStorage, next) == 0x08);
static_assert(offsetof(ListNodeStorage, prev) == 0x0C);

struct ListNodeInitOverlay
{
    ListNodeStorage* node;          // +0x00
    std::uint32_t field_04;         // +0x04
};

static_assert(offsetof(ListNodeInitOverlay, node) == 0x00);
static_assert(offsetof(ListNodeInitOverlay, field_04) == 0x04);

void ListNode_AllocateNode(std::uint32_t param_3);

ListNodeInitOverlay* __thiscall ListNode_Init(
    ListNodeInitOverlay* self,
    std::uint32_t /*param_2*/,
    std::uint32_t param_3)
{
    ListNode_AllocateNode(param_3);

    self->field_04 = 0;
    self->node->initializedFlag = 0;
    self->node->field_04 = 0;
    self->node->next = self->node;
    self->node->prev = self->node;

    return self;
}
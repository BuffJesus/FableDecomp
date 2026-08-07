#include <cstddef>
#include <cstdint>

struct ListNodeStorage_J {
    std::uint8_t initializedFlag;   // 0x00
    std::uint8_t padding_01[3];     // 0x01
    std::uint32_t value_04;         // 0x04
    ListNodeStorage_J* next;        // 0x08
    ListNodeStorage_J* prev;        // 0x0C
};

static_assert(offsetof(ListNodeStorage_J, initializedFlag) == 0x00);
static_assert(offsetof(ListNodeStorage_J, value_04) == 0x04);
static_assert(offsetof(ListNodeStorage_J, next) == 0x08);
static_assert(offsetof(ListNodeStorage_J, prev) == 0x0C);

struct ListNodeOwner_J {
    ListNodeStorage_J* node;        // 0x00
    std::uint32_t value_04;         // 0x04
};

static_assert(offsetof(ListNodeOwner_J, node) == 0x00);
static_assert(offsetof(ListNodeOwner_J, value_04) == 0x04);

void __stdcall ListNode_AllocateNode_Variant_J(std::uint32_t param_3);

ListNodeOwner_J* __thiscall ListNode_Init_J(
    ListNodeOwner_J* self,
    std::uint32_t /*param_2*/,
    std::uint32_t param_3)
{
    ListNode_AllocateNode_Variant_J(param_3);

    self->value_04 = 0;

    self->node->initializedFlag = 0;
    self->node->value_04 = 0;
    self->node->next = self->node;
    self->node->prev = self->node;

    return self;
}
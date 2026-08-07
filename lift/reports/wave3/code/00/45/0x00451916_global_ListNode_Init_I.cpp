#include <cstddef>
#include <cstdint>

struct ListNode_ObjectOverlay
{
    std::uint8_t* node;   // 0x00
    std::int32_t count;   // 0x04
};
static_assert(offsetof(ListNode_ObjectOverlay, node) == 0x00);
static_assert(offsetof(ListNode_ObjectOverlay, count) == 0x04);

struct ListNode_NodeOverlay
{
    std::uint8_t flag;        // 0x00
    std::uint8_t pad_01[3];   // 0x01
    std::uint32_t value;      // 0x04
    std::uint8_t* next;       // 0x08
    std::uint8_t* prev;       // 0x0C
};
static_assert(offsetof(ListNode_NodeOverlay, flag) == 0x00);
static_assert(offsetof(ListNode_NodeOverlay, value) == 0x04);
static_assert(offsetof(ListNode_NodeOverlay, next) == 0x08);
static_assert(offsetof(ListNode_NodeOverlay, prev) == 0x0C);

extern "C" void __cdecl ListNode_AllocateNode_Variant_I(std::uint32_t param_3);

int* __thiscall ListNode_Init_I(int* this_, std::uint32_t /*param_2*/, std::uint32_t param_3)
{
    ListNode_AllocateNode_Variant_I(param_3);

    auto* self = reinterpret_cast<ListNode_ObjectOverlay*>(this_);
    self->count = 0;

    auto* node = reinterpret_cast<ListNode_NodeOverlay*>(self->node);
    node->flag = 0;
    node->value = 0;
    node->next = self->node;
    node->prev = self->node;

    return this_;
}
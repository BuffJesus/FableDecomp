#include <cstddef>
#include <cstdint>

struct ListNode_Init_O_NodeOverlay {
    std::uint8_t field_0x0;
    std::uint8_t pad_0x1[3];
    std::uint32_t field_0x4;
    ListNode_Init_O_NodeOverlay* field_0x8;
    ListNode_Init_O_NodeOverlay* field_0xC;
};

static_assert(offsetof(ListNode_Init_O_NodeOverlay, field_0x0) == 0x0);
static_assert(offsetof(ListNode_Init_O_NodeOverlay, field_0x4) == 0x4);
static_assert(offsetof(ListNode_Init_O_NodeOverlay, field_0x8) == 0x8);
static_assert(offsetof(ListNode_Init_O_NodeOverlay, field_0xC) == 0xC);

struct ListNode_Init_O_ThisOverlay {
    ListNode_Init_O_NodeOverlay* field_0x0;
    std::uint32_t field_0x4;
};

static_assert(offsetof(ListNode_Init_O_ThisOverlay, field_0x0) == 0x0);
static_assert(offsetof(ListNode_Init_O_ThisOverlay, field_0x4) == 0x4);

// Binary evidence shows the callee pops its single stack argument.
extern "C" void __stdcall ListNode_AllocateNode_Variant_O(std::uint32_t param_3);

ListNode_Init_O_ThisOverlay* __thiscall ListNode_Init_O(
    ListNode_Init_O_ThisOverlay* this_,
    std::uint32_t /*param_2*/,
    std::uint32_t param_3)
{
    ListNode_AllocateNode_Variant_O(param_3);

    this_->field_0x4 = 0;

    ListNode_Init_O_NodeOverlay* const node = this_->field_0x0;
    node->field_0x0 = 0;
    node->field_0x4 = 0;
    node->field_0x8 = node;
    node->field_0xC = node;

    return this_;
}
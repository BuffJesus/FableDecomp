#include <cstddef>
#include <cstdint>

struct ListNodeOwnerB {
    std::int32_t node;
    std::int32_t state;
};
static_assert(offsetof(ListNodeOwnerB, node) == 0x0);
static_assert(offsetof(ListNodeOwnerB, state) == 0x4);

struct ListNodeOverlayB {
    std::uint8_t flag;
    std::uint8_t pad_01[3];
    std::int32_t value4;
    std::int32_t next;
    std::int32_t prev;
};
static_assert(offsetof(ListNodeOverlayB, flag) == 0x0);
static_assert(offsetof(ListNodeOverlayB, value4) == 0x4);
static_assert(offsetof(ListNodeOverlayB, next) == 0x8);
static_assert(offsetof(ListNodeOverlayB, prev) == 0xC);

extern "C" void __cdecl ListNode_AllocateNode_Variant_B(std::uint32_t param_3);

std::int32_t* __thiscall ListNode_Init_B(std::int32_t* param_1, std::uint32_t /*param_2*/, std::uint32_t param_3)
{
    ListNode_AllocateNode_Variant_B(param_3);

    auto* const self = reinterpret_cast<ListNodeOwnerB*>(param_1);
    self->state = 0;

    auto* const node = reinterpret_cast<ListNodeOverlayB*>(self->node);
    node->flag = 0;
    node->value4 = 0;
    node->next = self->node;
    node->prev = self->node;

    return param_1;
}
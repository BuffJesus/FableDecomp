#include <cstddef>
#include <cstdint>

struct ListNodeStorage_H {
    std::uint8_t field_0;
    std::uint8_t pad_01[3];
    std::uint32_t field_4;
    std::uint32_t field_8;
    std::uint32_t field_C;
};

static_assert(offsetof(ListNodeStorage_H, field_0) == 0x0);
static_assert(offsetof(ListNodeStorage_H, field_4) == 0x4);
static_assert(offsetof(ListNodeStorage_H, field_8) == 0x8);
static_assert(offsetof(ListNodeStorage_H, field_C) == 0xC);

struct ListNodeInitOverlay_H {
    std::uint32_t node;
    std::uint32_t field_4;
};

static_assert(offsetof(ListNodeInitOverlay_H, node) == 0x0);
static_assert(offsetof(ListNodeInitOverlay_H, field_4) == 0x4);

void ListNode_AllocateNode_Variant_H(std::uint32_t param_3);

ListNodeInitOverlay_H* __thiscall ListNode_Init_H(
    ListNodeInitOverlay_H* self,
    std::uint32_t /*param_2*/,
    std::uint32_t param_3)
{
    ListNode_AllocateNode_Variant_H(param_3);

    self->field_4 = 0;

    auto* const node = reinterpret_cast<ListNodeStorage_H*>(self->node);
    node->field_0 = 0;
    node->field_4 = 0;
    node->field_8 = self->node;
    node->field_C = self->node;

    return self;
}
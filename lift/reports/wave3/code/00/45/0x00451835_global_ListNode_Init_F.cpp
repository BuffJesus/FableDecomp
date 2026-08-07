#include <cstddef>
#include <cstdint>

struct ListNodeOverlay_F {
    std::uint8_t field_00;
    std::uint8_t pad_01[3];
    std::uint32_t field_04;
    ListNodeOverlay_F* field_08;
    ListNodeOverlay_F* field_0C;
};

static_assert(offsetof(ListNodeOverlay_F, field_00) == 0x00);
static_assert(offsetof(ListNodeOverlay_F, field_04) == 0x04);
static_assert(offsetof(ListNodeOverlay_F, field_08) == 0x08);
static_assert(offsetof(ListNodeOverlay_F, field_0C) == 0x0C);

struct ListNodeInitContext_F {
    ListNodeOverlay_F* field_00;
    std::uint32_t field_04;
};

static_assert(offsetof(ListNodeInitContext_F, field_00) == 0x00);
static_assert(offsetof(ListNodeInitContext_F, field_04) == 0x04);

extern void ListNode_AllocateNode_Variant_F(std::uint32_t param_3);

ListNodeInitContext_F* __thiscall ListNode_Init_F(
    ListNodeInitContext_F* self,
    std::uint32_t /*param_2*/,
    std::uint32_t param_3)
{
    ListNode_AllocateNode_Variant_F(param_3);
    self->field_04 = 0;

    self->field_00->field_00 = 0;
    self->field_00->field_04 = 0;
    self->field_00->field_08 = self->field_00;
    self->field_00->field_0C = self->field_00;

    return self;
}
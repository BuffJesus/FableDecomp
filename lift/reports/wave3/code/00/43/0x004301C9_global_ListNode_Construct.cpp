#include <cstddef>
#include <cstdint>

void __stdcall Stack_Initialize(std::uint32_t value);

struct ListNodeStorage {
    std::uint8_t field_0x00;
    std::uint8_t pad_0x01[3];
    std::uint32_t field_0x04;
    ListNodeStorage* field_0x08;
    ListNodeStorage* field_0x0C;
};

static_assert(offsetof(ListNodeStorage, field_0x00) == 0x00);
static_assert(offsetof(ListNodeStorage, field_0x04) == 0x04);
static_assert(offsetof(ListNodeStorage, field_0x08) == 0x08);
static_assert(offsetof(ListNodeStorage, field_0x0C) == 0x0C);

struct ListNodeConstructOverlay {
    ListNodeStorage* node;
    std::uint32_t field_0x04;
};

static_assert(offsetof(ListNodeConstructOverlay, node) == 0x00);
static_assert(offsetof(ListNodeConstructOverlay, field_0x04) == 0x04);

int* __thiscall ListNode_Construct(
    ListNodeConstructOverlay* self,
    std::uint32_t /*unused*/,
    std::uint32_t stackInitializeArg)
{
    Stack_Initialize(stackInitializeArg);

    self->field_0x04 = 0;
    self->node->field_0x00 = 0;
    self->node->field_0x04 = 0;
    self->node->field_0x08 = self->node;
    self->node->field_0x0C = self->node;

    return reinterpret_cast<int*>(self);
}
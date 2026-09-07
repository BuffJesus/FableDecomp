#include <cstddef>
#include <cstdint>

struct CTCCarryableNodeOverlay
{
    std::uint8_t pad_00[0x04];
    CTCCarryableNodeOverlay* field_04;
    CTCCarryableNodeOverlay* field_08;
    CTCCarryableNodeOverlay* field_0C;
};

static_assert(offsetof(CTCCarryableNodeOverlay, field_04) == 0x04);
static_assert(offsetof(CTCCarryableNodeOverlay, field_08) == 0x08);
static_assert(offsetof(CTCCarryableNodeOverlay, field_0C) == 0x0C);

struct CTCCarryableOverlay
{
    CTCCarryableNodeOverlay* field_00;
    std::int32_t field_04;
};

static_assert(offsetof(CTCCarryableOverlay, field_00) == 0x00);
static_assert(offsetof(CTCCarryableOverlay, field_04) == 0x04);

extern CTCCarryableNodeOverlay* __cdecl CTCCarryable_CloneDeep(
    CTCCarryableNodeOverlay* source,
    CTCCarryableNodeOverlay* parent);

CTCCarryable* __thiscall CTCCarryable_assign_copy(CTCCarryable* self, CTCCarryable* other)
{
    auto* const selfOverlay = reinterpret_cast<CTCCarryableOverlay*>(self);
    auto* const otherOverlay = reinterpret_cast<CTCCarryableOverlay*>(other);

    if (self != other)
    {
        CTCCarryable::OnKill(self);
        selfOverlay->field_04 = 0;

        if (otherOverlay->field_00->field_04 == nullptr)
        {
            selfOverlay->field_00->field_04 = nullptr;
            selfOverlay->field_00->field_08 = selfOverlay->field_00;
            selfOverlay->field_00->field_0C = selfOverlay->field_00;
        }
        else
        {
            CTCCarryableNodeOverlay* const root = selfOverlay->field_00;
            root->field_04 = CTCCarryable_CloneDeep(otherOverlay->field_00->field_04, root);

            CTCCarryableNodeOverlay* node = selfOverlay->field_00->field_04;
            CTCCarryableNodeOverlay* last;
            do
            {
                last = node;
                node = last->field_08;
            } while (node != nullptr);
            selfOverlay->field_00->field_08 = last;

            node = selfOverlay->field_00->field_04;
            do
            {
                last = node;
                node = last->field_0C;
            } while (node != nullptr);
            selfOverlay->field_00->field_0C = last;

            selfOverlay->field_04 = otherOverlay->field_04;
        }
    }

    return self;
}
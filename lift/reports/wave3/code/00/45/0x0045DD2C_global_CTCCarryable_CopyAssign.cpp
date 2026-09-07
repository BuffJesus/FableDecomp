#include <cstddef>
#include <cstdint>

struct CTCCarryable;
struct CTCCarryableNodeOverlay;

struct CTCCarryableTreeOverlay {
    std::uint32_t field_0x00;
    CTCCarryableNodeOverlay* field_0x04;
    void* field_0x08;
    void* field_0x0C;
};

struct CTCCarryableNodeOverlay {
    std::uint8_t pad_0x00[0x08];
    CTCCarryableNodeOverlay* field_0x08;
    CTCCarryableNodeOverlay* field_0x0C;
};

struct CTCCarryableOverlay {
    CTCCarryableTreeOverlay* field_0x00;
    std::uint32_t field_0x04;
};

static_assert(offsetof(CTCCarryableTreeOverlay, field_0x04) == 0x04);
static_assert(offsetof(CTCCarryableTreeOverlay, field_0x08) == 0x08);
static_assert(offsetof(CTCCarryableTreeOverlay, field_0x0C) == 0x0C);
static_assert(offsetof(CTCCarryableNodeOverlay, field_0x08) == 0x08);
static_assert(offsetof(CTCCarryableNodeOverlay, field_0x0C) == 0x0C);
static_assert(offsetof(CTCCarryableOverlay, field_0x00) == 0x00);
static_assert(offsetof(CTCCarryableOverlay, field_0x04) == 0x04);

CTCCarryableNodeOverlay* __thiscall CTCCarryable_CloneDeep(
    CTCCarryableNodeOverlay* root,
    CTCCarryableTreeOverlay* owner);

struct CTCCarryable {
    void __thiscall OnKill();
};

CTCCarryable* __thiscall CTCCarryable_CopyAssign(CTCCarryable* self, CTCCarryable* other) {
    if (self != other) {
        self->OnKill();

        auto* const selfOverlay = reinterpret_cast<CTCCarryableOverlay*>(self);
        auto* const otherOverlay = reinterpret_cast<CTCCarryableOverlay*>(other);

        selfOverlay->field_0x04 = 0;

        if (otherOverlay->field_0x00->field_0x04 == nullptr) {
            selfOverlay->field_0x00->field_0x04 = nullptr;
            selfOverlay->field_0x00->field_0x08 = selfOverlay->field_0x00;
            selfOverlay->field_0x00->field_0x0C = selfOverlay->field_0x00;
        } else {
            CTCCarryableTreeOverlay* const tree = selfOverlay->field_0x00;
            tree->field_0x04 = CTCCarryable_CloneDeep(otherOverlay->field_0x00->field_0x04, tree);

            CTCCarryableNodeOverlay* node = selfOverlay->field_0x00->field_0x04;
            CTCCarryableNodeOverlay* prev;
            do {
                prev = node;
                node = prev->field_0x08;
            } while (node != nullptr);
            selfOverlay->field_0x00->field_0x08 = prev;

            node = selfOverlay->field_0x00->field_0x04;
            do {
                prev = node;
                node = prev->field_0x0C;
            } while (node != nullptr);
            selfOverlay->field_0x00->field_0x0C = prev;

            selfOverlay->field_0x04 = reinterpret_cast<CTCCarryableOverlay*>(other)->field_0x04;
        }
    }

    return self;
}
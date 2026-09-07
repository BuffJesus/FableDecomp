#include <cstddef>
#include <cstdint>

class CTCCarryable {
public:
    void OnKill();
};

struct CTCCarryable_TreeNodeOverlay {
    std::byte pad_0[0x8];
    void* field_8;
    void* field_C;
};
static_assert(offsetof(CTCCarryable_TreeNodeOverlay, field_8) == 0x8);
static_assert(offsetof(CTCCarryable_TreeNodeOverlay, field_C) == 0xC);

struct CTCCarryable_TreeHeadOverlay {
    std::byte pad_0[0x4];
    void* field_4;
    void* field_8;
    void* field_C;
};
static_assert(offsetof(CTCCarryable_TreeHeadOverlay, field_4) == 0x4);
static_assert(offsetof(CTCCarryable_TreeHeadOverlay, field_8) == 0x8);
static_assert(offsetof(CTCCarryable_TreeHeadOverlay, field_C) == 0xC);

struct CTCCarryable_Overlay {
    CTCCarryable_TreeHeadOverlay* field_0;
    std::uint32_t field_4;
};
static_assert(offsetof(CTCCarryable_Overlay, field_0) == 0x0);
static_assert(offsetof(CTCCarryable_Overlay, field_4) == 0x4);

extern void* Tree_Clone_Recursive(void* sourceRoot, void* ownerHead);

CTCCarryable* CTCCarryable_AssignmentOperator(CTCCarryable* self, CTCCarryable* other)
{
    if (self != other) {
        self->CTCCarryable::OnKill();

        auto* const selfView = reinterpret_cast<CTCCarryable_Overlay*>(self);
        auto* const otherView = reinterpret_cast<CTCCarryable_Overlay*>(other);

        selfView->field_4 = 0;

        if (otherView->field_0->field_4 == nullptr) {
            selfView->field_0->field_4 = nullptr;
            selfView->field_0->field_8 = selfView->field_0;
            selfView->field_0->field_C = selfView->field_0;
        } else {
            CTCCarryable_TreeHeadOverlay* const selfHead = selfView->field_0;
            selfHead->field_4 = Tree_Clone_Recursive(otherView->field_0->field_4, selfHead);

            void* current = selfHead->field_4;
            void* last = nullptr;
            do {
                last = current;
                current = reinterpret_cast<CTCCarryable_TreeNodeOverlay*>(last)->field_8;
            } while (current != nullptr);
            selfHead->field_8 = last;

            current = selfHead->field_4;
            do {
                last = current;
                current = reinterpret_cast<CTCCarryable_TreeNodeOverlay*>(last)->field_C;
            } while (current != nullptr);
            selfHead->field_C = last;

            selfView->field_4 = otherView->field_4;
        }
    }

    return self;
}
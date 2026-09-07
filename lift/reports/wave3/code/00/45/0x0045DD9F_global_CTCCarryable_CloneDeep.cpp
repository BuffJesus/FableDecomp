#include <cstddef>
#include <cstdint>

struct CTCCarryableNodeOverlay {
    std::uint8_t type;
    std::uint8_t padding_01[3];
    CTCCarryableNodeOverlay* backlink;
    CTCCarryableNodeOverlay* next;
    CTCCarryableNodeOverlay* child;
};

static_assert(offsetof(CTCCarryableNodeOverlay, type) == 0x00);
static_assert(offsetof(CTCCarryableNodeOverlay, backlink) == 0x04);
static_assert(offsetof(CTCCarryableNodeOverlay, next) == 0x08);
static_assert(offsetof(CTCCarryableNodeOverlay, child) == 0x0C);
static_assert(sizeof(CTCCarryableNodeOverlay) == 0x10);

struct CTCCarryableCloneDeepContext;

void* TreeNode_AllocData_Byte(void* sourceData);

CTCCarryableNodeOverlay* CTCCarryable_CloneDeep(
    CTCCarryableCloneDeepContext* self,
    CTCCarryableNodeOverlay* source,
    CTCCarryableNodeOverlay* backlink
) {
    auto* const clone = static_cast<CTCCarryableNodeOverlay*>(
        TreeNode_AllocData_Byte(reinterpret_cast<std::uint8_t*>(source) + 0x10)
    );

    clone->next = nullptr;
    clone->child = nullptr;
    clone->type = source->type;
    clone->backlink = backlink;

    if (source->child != nullptr) {
        clone->child = CTCCarryable_CloneDeep(self, source->child, clone);
    }

    CTCCarryableNodeOverlay* tail = clone;
    for (CTCCarryableNodeOverlay* sibling = source->next; sibling != nullptr; sibling = sibling->next) {
        auto* const siblingClone = static_cast<CTCCarryableNodeOverlay*>(
            TreeNode_AllocData_Byte(reinterpret_cast<std::uint8_t*>(sibling) + 0x10)
        );

        siblingClone->next = nullptr;
        siblingClone->child = nullptr;
        siblingClone->type = sibling->type;
        tail->next = siblingClone;
        siblingClone->backlink = tail;

        if (sibling->child != nullptr) {
            siblingClone->child = CTCCarryable_CloneDeep(self, sibling->child, siblingClone);
        }

        tail = siblingClone;
    }

    return clone;
}
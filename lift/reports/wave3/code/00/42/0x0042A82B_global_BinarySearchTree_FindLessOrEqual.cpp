#include <cstddef>
#include <cstdint>

struct BinarySearchTreeNodeOverlay {
    std::uint8_t pad_00[0x08];
    BinarySearchTreeNodeOverlay* left;   // 0x08
    BinarySearchTreeNodeOverlay* right;  // 0x0C
    std::uint32_t key;                   // 0x10
};

static_assert(offsetof(BinarySearchTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(BinarySearchTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(BinarySearchTreeNodeOverlay, key) == 0x10);

struct BinarySearchTreeHeaderOverlay {
    std::uint8_t pad_00[0x04];
    BinarySearchTreeNodeOverlay* root;   // 0x04
};

static_assert(offsetof(BinarySearchTreeHeaderOverlay, root) == 0x04);

struct BinarySearchTreeOverlay {
    BinarySearchTreeHeaderOverlay* header; // 0x00
};

static_assert(offsetof(BinarySearchTreeOverlay, header) == 0x00);

BinarySearchTreeNodeOverlay* __thiscall BinarySearchTree_FindLessOrEqual(
    BinarySearchTreeOverlay* this_,
    const std::uint32_t* targetValue)
{
    BinarySearchTreeHeaderOverlay* const header = this_->header;
    BinarySearchTreeNodeOverlay* candidate =
        reinterpret_cast<BinarySearchTreeNodeOverlay*>(header);

    if (header->root != nullptr) {
        BinarySearchTreeNodeOverlay* current = header->root;
        do {
            BinarySearchTreeNodeOverlay* next;
            if (current->key < *targetValue) {
                next = current->right;
            } else {
                next = current->left;
                candidate = current;
            }
            current = next;
        } while (next != nullptr);
    }

    if ((candidate == reinterpret_cast<BinarySearchTreeNodeOverlay*>(header)) ||
        (*targetValue < candidate->key)) {
        candidate = reinterpret_cast<BinarySearchTreeNodeOverlay*>(header);
    }

    return candidate;
}
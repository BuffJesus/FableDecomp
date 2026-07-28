#include <cstddef>
#include <cstdint>

struct BinarySearchTreeNodeOverlay {
    std::uint8_t pad_00[0x4];
    BinarySearchTreeNodeOverlay* parent_or_root; // +0x04: root when this is the header/sentinel
    BinarySearchTreeNodeOverlay* left;           // +0x08
    BinarySearchTreeNodeOverlay* right;          // +0x0C
    std::uint32_t key;                           // +0x10
};

static_assert(offsetof(BinarySearchTreeNodeOverlay, parent_or_root) == 0x04);
static_assert(offsetof(BinarySearchTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(BinarySearchTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(BinarySearchTreeNodeOverlay, key) == 0x10);

struct BinarySearchTreeOverlay {
    BinarySearchTreeNodeOverlay* header; // +0x00
};

static_assert(offsetof(BinarySearchTreeOverlay, header) == 0x00);

BinarySearchTreeNodeOverlay* __thiscall BinarySearchTree_FindExact(
    BinarySearchTreeOverlay* self,
    const std::uint32_t* key)
{
    BinarySearchTreeNodeOverlay* candidate = self->header;
    BinarySearchTreeNodeOverlay* current = candidate->parent_or_root;

    if (current != nullptr) {
        const std::uint32_t searchKey = *key;

        do {
            if (current->key < searchKey) {
                current = current->right;
            } else {
                candidate = current;
                current = current->left;
            }
        } while (current != nullptr);
    }

    return candidate;
}
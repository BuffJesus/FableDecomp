#include <cstddef>
#include <cstdint>

struct BinarySearchTreeNodeOverlay {
    std::byte _pad0[0x08];
    BinarySearchTreeNodeOverlay* left;   // 0x08
    BinarySearchTreeNodeOverlay* right;  // 0x0C
    std::uint32_t key;                   // 0x10
};

static_assert(offsetof(BinarySearchTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(BinarySearchTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(BinarySearchTreeNodeOverlay, key) == 0x10);

struct BinarySearchTreeHeaderOverlay {
    std::byte _pad0[0x04];
    BinarySearchTreeNodeOverlay* root;   // 0x04
};

static_assert(offsetof(BinarySearchTreeHeaderOverlay, root) == 0x04);

void* __thiscall BinarySearchTree_FindEqualOrGreater(
    void* const* this_,
    const std::uint32_t* key)
{
    void* result = *this_;
    const auto* tree = static_cast<const BinarySearchTreeHeaderOverlay*>(result);

    if (tree->root != nullptr) {
        BinarySearchTreeNodeOverlay* current = tree->root;
        do {
            BinarySearchTreeNodeOverlay* next;
            if (current->key < *key) {
                next = current->right;
            } else {
                next = current->left;
                result = current;
            }
            current = next;
        } while (next != nullptr);
    }

    return result;
}
#include <cstddef>
#include <cstdint>

struct BinarySearchTreeHeaderOverlay {
    std::byte pad_00[0x04];
    std::intptr_t root; // 0x04
};
static_assert(offsetof(BinarySearchTreeHeaderOverlay, root) == 0x04);

struct BinarySearchTreeNodeOverlay {
    std::byte pad_00[0x08];
    std::intptr_t left;  // 0x08
    std::intptr_t right; // 0x0C
    std::uint32_t key;   // 0x10
};
static_assert(offsetof(BinarySearchTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(BinarySearchTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(BinarySearchTreeNodeOverlay, key) == 0x10);

std::intptr_t __thiscall BinarySearchTree_FindByUintComparator(std::intptr_t* self, const std::uint32_t* key) {
    std::intptr_t result = *self;
    const auto* header = reinterpret_cast<const BinarySearchTreeHeaderOverlay*>(result);

    if (header->root != 0) {
        std::intptr_t current = header->root;

        do {
            const auto* node = reinterpret_cast<const BinarySearchTreeNodeOverlay*>(current);
            std::intptr_t next;

            if (*key < node->key) {
                next = node->left;
                result = current;
            } else {
                next = node->right;
            }

            current = next;
        } while (current != 0);
    }

    return result;
}
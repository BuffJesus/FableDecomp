#include <cstddef>
#include <cstdint>

struct BinarySearchTreeNodeOverlay {
    std::byte pad_0x0[0x8];
    BinarySearchTreeNodeOverlay* left_at_0x8;
    BinarySearchTreeNodeOverlay* right_at_0xC;
    std::uint32_t key_at_0x10;
};

static_assert(offsetof(BinarySearchTreeNodeOverlay, left_at_0x8) == 0x8);
static_assert(offsetof(BinarySearchTreeNodeOverlay, right_at_0xC) == 0xC);
static_assert(offsetof(BinarySearchTreeNodeOverlay, key_at_0x10) == 0x10);

struct BinarySearchTreeHeaderOverlay {
    std::byte pad_0x0[0x4];
    BinarySearchTreeNodeOverlay* root_at_0x4;
};

static_assert(offsetof(BinarySearchTreeHeaderOverlay, root_at_0x4) == 0x4);

struct BinarySearchTreeThisOverlay {
    BinarySearchTreeHeaderOverlay* header_at_0x0;
};

static_assert(offsetof(BinarySearchTreeThisOverlay, header_at_0x0) == 0x0);

int __thiscall BinarySearchTree_FindGreaterOrEqual(
    BinarySearchTreeThisOverlay* this_,
    const std::uint32_t* key)
{
    BinarySearchTreeHeaderOverlay* const header = this_->header_at_0x0;
    int result = reinterpret_cast<int>(header);

    if (header->root_at_0x4 != nullptr) {
        BinarySearchTreeNodeOverlay* current = header->root_at_0x4;
        do {
            BinarySearchTreeNodeOverlay* next;
            if (current->key_at_0x10 < *key) {
                next = current->right_at_0xC;
            } else {
                next = current->left_at_0x8;
                result = reinterpret_cast<int>(current);
            }
            current = next;
        } while (next != nullptr);
    }

    return result;
}
#include <cstddef>
#include <cstdint>

struct BinarySearchTreeNodeOverlay {
    std::uint8_t pad_0x00[0x08];
    BinarySearchTreeNodeOverlay* left;   // 0x08
    BinarySearchTreeNodeOverlay* right;  // 0x0C
    int key;                             // 0x10
};

static_assert(offsetof(BinarySearchTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(BinarySearchTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(BinarySearchTreeNodeOverlay, key) == 0x10);

struct BinarySearchTreeHeaderOverlay {
    std::uint8_t pad_0x00[0x04];
    BinarySearchTreeNodeOverlay* root;   // 0x04
};

static_assert(offsetof(BinarySearchTreeHeaderOverlay, root) == 0x04);

struct BinarySearchTreeOverlay {
    BinarySearchTreeHeaderOverlay* header; // 0x00
};

static_assert(offsetof(BinarySearchTreeOverlay, header) == 0x00);

int __thiscall BinarySearchTree_FindByIntKey(BinarySearchTreeOverlay* self, const int* key)
{
    BinarySearchTreeHeaderOverlay* result = self->header;

    if (result->root != nullptr) {
        BinarySearchTreeNodeOverlay* node = result->root;
        do {
            BinarySearchTreeNodeOverlay* next;
            if (node->key < *key) {
                next = node->right;
            } else {
                next = node->left;
                result = reinterpret_cast<BinarySearchTreeHeaderOverlay*>(node);
            }
            node = next;
        } while (next != nullptr);
    }

    return reinterpret_cast<int>(result);
}
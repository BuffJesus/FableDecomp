#include <cstddef>
#include <cstdint>

struct BinarySearchTreeNodeOverlay {
    std::uint8_t pad00[0x08];
    BinarySearchTreeNodeOverlay* left;   // 0x08
    BinarySearchTreeNodeOverlay* right;  // 0x0C
    int key;                             // 0x10
};

static_assert(offsetof(BinarySearchTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(BinarySearchTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(BinarySearchTreeNodeOverlay, key) == 0x10);

struct BinarySearchTreeHeaderOverlay {
    std::uint8_t pad00[0x04];
    BinarySearchTreeNodeOverlay* root;   // 0x04
};

static_assert(offsetof(BinarySearchTreeHeaderOverlay, root) == 0x04);

struct BinarySearchTreeOverlay {
    BinarySearchTreeHeaderOverlay* header; // 0x00
};

static_assert(offsetof(BinarySearchTreeOverlay, header) == 0x00);

BinarySearchTreeNodeOverlay* __thiscall BinarySearchTree_FindNode(
    BinarySearchTreeOverlay* self,
    const int* keyPtr)
{
    BinarySearchTreeHeaderOverlay* const header = self->header;
    BinarySearchTreeHeaderOverlay* candidate = header;

    if (header->root != nullptr) {
        BinarySearchTreeNodeOverlay* current = header->root;
        do {
            BinarySearchTreeNodeOverlay* next;
            if (current->key < *keyPtr) {
                next = current->right;
            } else {
                next = current->left;
                candidate = reinterpret_cast<BinarySearchTreeHeaderOverlay*>(current);
            }
            current = next;
        } while (next != nullptr);
    }

    if ((candidate == header) || (*keyPtr < reinterpret_cast<BinarySearchTreeNodeOverlay*>(candidate)->key)) {
        candidate = header;
    }

    return reinterpret_cast<BinarySearchTreeNodeOverlay*>(candidate);
}
#include <cstddef>
#include <cstdint>

struct RBTreeNodeOverlay {
    std::byte pad0[0x08];
    RBTreeNodeOverlay* left;   // 0x08
    RBTreeNodeOverlay* right;  // 0x0C
    std::uint32_t key;         // 0x10
};
static_assert(offsetof(RBTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, key) == 0x10);

struct RBTreeHeaderOverlay {
    std::byte pad0[0x04];
    RBTreeNodeOverlay* root;          // 0x04
    RBTreeNodeOverlay* rightSentinel; // 0x08
};
static_assert(offsetof(RBTreeHeaderOverlay, root) == 0x04);
static_assert(offsetof(RBTreeHeaderOverlay, rightSentinel) == 0x08);

struct RBTreeOverlay {
    RBTreeHeaderOverlay* header; // 0x00
};
static_assert(offsetof(RBTreeOverlay, header) == 0x00);

struct RBTreeInsertResult {
    RBTreeNodeOverlay* node; // 0x00
    bool inserted;           // 0x04
};
static_assert(offsetof(RBTreeInsertResult, node) == 0x00);
static_assert(offsetof(RBTreeInsertResult, inserted) == 0x04);

extern RBTreeNodeOverlay* __cdecl RBTree__GetMaxNode(RBTreeNodeOverlay* node);
extern RBTreeNodeOverlay** __cdecl BinarySearchTree_InsertAndBalance(
    std::uint32_t** keySlot,
    RBTreeNodeOverlay* linkOrNode,
    RBTreeNodeOverlay* parent,
    std::uint32_t* key,
    int zeroFlag);

RBTreeInsertResult* __thiscall RBTree_InsertOrReplace(
    RBTreeOverlay* self,
    RBTreeInsertResult* result,
    std::uint32_t* key)
{
    RBTreeHeaderOverlay* const header = self->header;
    RBTreeNodeOverlay* current = header->root;
    bool goLeft = true;
    RBTreeNodeOverlay* parent = reinterpret_cast<RBTreeNodeOverlay*>(header);

    if (current != nullptr) {
        do {
            parent = current;
            goLeft = *key < parent->key;
            if (goLeft) {
                current = parent->left;
            } else {
                current = parent->right;
            }
        } while (current != nullptr);
    }

    RBTreeNodeOverlay* candidate = parent;
    RBTreeNodeOverlay* insertLink = current;

    if (goLeft) {
        insertLink = parent;
        if (parent == header->rightSentinel) {
            goto insert_path;
        }
        candidate = RBTree__GetMaxNode(parent);
    }

    insertLink = current;
    if (*key <= candidate->key) {
        result->node = candidate;
        result->inserted = false;
        return result;
    }

insert_path:
    {
        RBTreeNodeOverlay** const insertedNode =
            BinarySearchTree_InsertAndBalance(&key, insertLink, parent, key, 0);
        result->node = *insertedNode;
        result->inserted = true;
        return result;
    }
}
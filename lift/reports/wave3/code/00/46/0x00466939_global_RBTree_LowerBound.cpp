#include <cstddef>
#include <cstdint>

struct RBTreeNodeOverlay {
    std::uint8_t pad0[0x08];
    RBTreeNodeOverlay* left;   // 0x08
    RBTreeNodeOverlay* right;  // 0x0C
    int value;                 // 0x10
};
static_assert(offsetof(RBTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, value) == 0x10);

struct RBTreeHeaderOverlay {
    std::uint8_t pad0[0x04];
    RBTreeNodeOverlay* root;       // 0x04
    RBTreeNodeOverlay* rightmost;  // 0x08
};
static_assert(offsetof(RBTreeHeaderOverlay, root) == 0x04);
static_assert(offsetof(RBTreeHeaderOverlay, rightmost) == 0x08);

struct RBTreeOverlay {
    RBTreeHeaderOverlay* header; // 0x00
};
static_assert(offsetof(RBTreeOverlay, header) == 0x00);

struct RBTreeLowerBoundResult {
    RBTreeNodeOverlay* node; // 0x00
    std::uint8_t inserted;   // 0x04
};
static_assert(offsetof(RBTreeLowerBoundResult, node) == 0x00);
static_assert(offsetof(RBTreeLowerBoundResult, inserted) == 0x04);

RBTreeNodeOverlay* __cdecl RBTree__GetMaxNode(RBTreeNodeOverlay* node);
RBTreeNodeOverlay** __cdecl RBTree_InsertNode(
    int** keySlot,
    RBTreeNodeOverlay* hint,
    RBTreeNodeOverlay* parent,
    int* key,
    int value);

RBTreeLowerBoundResult* __thiscall RBTree_LowerBound(
    RBTreeOverlay* self,
    RBTreeLowerBoundResult* result,
    int* key)
{
    RBTreeHeaderOverlay* const header = self->header;
    RBTreeNodeOverlay* current = header->root;
    bool wentLeft = true;
    RBTreeNodeOverlay* parent = reinterpret_cast<RBTreeNodeOverlay*>(header);

    if (current != nullptr) {
        do {
            parent = current;
            wentLeft = *key < parent->value;
            if (wentLeft) {
                current = parent->left;
            } else {
                current = parent->right;
            }
        } while (current != nullptr);
    }

    RBTreeNodeOverlay* candidate = parent;
    RBTreeNodeOverlay* insertHint;

    if (wentLeft) {
        insertHint = parent;
        if (parent == header->rightmost) {
            RBTreeNodeOverlay** const insertedNode =
                RBTree_InsertNode(&key, insertHint, parent, key, 0);
            result->node = *insertedNode;
            result->inserted = 1;
            return result;
        }

        candidate = RBTree__GetMaxNode(parent);
    }

    insertHint = current;
    if (*key <= candidate->value) {
        result->node = candidate;
        result->inserted = 0;
        return result;
    }

    RBTreeNodeOverlay** const insertedNode =
        RBTree_InsertNode(&key, insertHint, parent, key, 0);
    result->node = *insertedNode;
    result->inserted = 1;
    return result;
}
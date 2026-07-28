#include <cstddef>
#include <cstdint>

struct RBTreeNodeOverlay {
    std::uint8_t pad_00[0x08];
    RBTreeNodeOverlay* left;   // 0x08
    RBTreeNodeOverlay* right;  // 0x0C
    int key;                   // 0x10
};
static_assert(offsetof(RBTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, key) == 0x10);

struct RBTreeHeaderOverlay {
    std::uint8_t pad_00[0x04];
    RBTreeNodeOverlay* root;      // 0x04
    RBTreeNodeOverlay* leftmost;  // 0x08
};
static_assert(offsetof(RBTreeHeaderOverlay, root) == 0x04);
static_assert(offsetof(RBTreeHeaderOverlay, leftmost) == 0x08);

struct RBTreeOverlay {
    RBTreeHeaderOverlay* header;  // 0x00
};
static_assert(offsetof(RBTreeOverlay, header) == 0x00);

struct RBTreeInsertResultOverlay {
    RBTreeNodeOverlay* node;  // 0x00
    std::uint8_t inserted;    // 0x04
};
static_assert(offsetof(RBTreeInsertResultOverlay, node) == 0x00);
static_assert(offsetof(RBTreeInsertResultOverlay, inserted) == 0x04);

extern RBTreeNodeOverlay* __cdecl RBTree__GetMaxNode(RBTreeNodeOverlay* node);

extern RBTreeNodeOverlay** __thiscall BinaryTree_InsertNodeLeft(
    RBTreeOverlay* self,
    int** value_slot,
    RBTreeNodeOverlay* insert_at,
    RBTreeNodeOverlay* parent,
    int* value,
    int zero);

RBTreeInsertResultOverlay* __thiscall RBTree_InsertLeft(
    RBTreeOverlay* self,
    RBTreeInsertResultOverlay* result,
    int* value)
{
    RBTreeHeaderOverlay* const header = self->header;
    RBTreeNodeOverlay* current = header->root;
    bool went_left = true;
    RBTreeNodeOverlay* parent = reinterpret_cast<RBTreeNodeOverlay*>(header);

    if (current != nullptr) {
        do {
            parent = current;
            went_left = (*value < parent->key);
            if (went_left) {
                current = parent->left;
            } else {
                current = parent->right;
            }
        } while (current != nullptr);
    }

    RBTreeNodeOverlay* candidate = parent;
    RBTreeNodeOverlay* insert_at;

    if (went_left) {
        insert_at = parent;
        if (parent == header->leftmost) {
            RBTreeNodeOverlay** const inserted =
                BinaryTree_InsertNodeLeft(self, &value, insert_at, parent, value, 0);
            result->node = *inserted;
            result->inserted = 1;
            return result;
        }

        candidate = RBTree__GetMaxNode(parent);
    }

    insert_at = current;
    if (*value <= candidate->key) {
        result->node = candidate;
        result->inserted = 0;
        return result;
    }

    RBTreeNodeOverlay** const inserted =
        BinaryTree_InsertNodeLeft(self, &value, insert_at, parent, value, 0);
    result->node = *inserted;
    result->inserted = 1;
    return result;
}
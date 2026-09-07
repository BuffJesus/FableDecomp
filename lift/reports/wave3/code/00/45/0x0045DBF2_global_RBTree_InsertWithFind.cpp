#include <cstddef>

struct RBTreeNodeOverlay {
    int reserved0;
    int reserved4;
    RBTreeNodeOverlay* left;
    RBTreeNodeOverlay* right;
    int key;
};
static_assert(offsetof(RBTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, key) == 0x10);

struct RBTreeHeaderOverlay {
    int reserved0;
    RBTreeNodeOverlay* root;
    RBTreeNodeOverlay* anchor;
};
static_assert(offsetof(RBTreeHeaderOverlay, root) == 0x04);
static_assert(offsetof(RBTreeHeaderOverlay, anchor) == 0x08);

struct RBTreeThisOverlay {
    RBTreeHeaderOverlay* tree;
};
static_assert(offsetof(RBTreeThisOverlay, tree) == 0x00);

struct RBTreeInsertFindResultOverlay {
    RBTreeNodeOverlay* node;
    bool inserted;
};
static_assert(offsetof(RBTreeInsertFindResultOverlay, node) == 0x00);
static_assert(offsetof(RBTreeInsertFindResultOverlay, inserted) == 0x04);

extern "C" RBTreeNodeOverlay* __cdecl RBTree__GetMaxNode(RBTreeNodeOverlay* node);
extern "C" RBTreeNodeOverlay** __thiscall RBTree_InsertNewNode(
    RBTreeThisOverlay* self,
    int** value_ref,
    RBTreeNodeOverlay* arg2,
    RBTreeNodeOverlay* arg3,
    int* value,
    int zero
);

RBTreeInsertFindResultOverlay* __thiscall RBTree_InsertWithFind(
    RBTreeThisOverlay* self,
    RBTreeInsertFindResultOverlay* result,
    int* value
) {
    RBTreeHeaderOverlay* const tree = self->tree;
    RBTreeNodeOverlay* current = tree->root;
    bool go_left = true;
    RBTreeNodeOverlay* parent = reinterpret_cast<RBTreeNodeOverlay*>(tree);

    if (current != nullptr) {
        do {
            parent = current;
            go_left = *value < parent->key;
            if (go_left) {
                current = parent->left;
            } else {
                current = parent->right;
            }
        } while (current != nullptr);
    }

    RBTreeNodeOverlay* candidate = parent;
    if (go_left) {
        RBTreeNodeOverlay* insert_hint = parent;
        if (parent == reinterpret_cast<RBTreeNodeOverlay*>(tree->anchor)) {
            int* value_ref = value;
            RBTreeNodeOverlay** inserted_node =
                RBTree_InsertNewNode(self, &value_ref, insert_hint, parent, value, 0);
            result->node = *inserted_node;
            result->inserted = true;
            return result;
        }
        candidate = RBTree__GetMaxNode(parent);
    }

    RBTreeNodeOverlay* insert_hint = current;
    if (*value <= candidate->key) {
        result->node = candidate;
        result->inserted = false;
        return result;
    }

    int* value_ref = value;
    RBTreeNodeOverlay** inserted_node =
        RBTree_InsertNewNode(self, &value_ref, insert_hint, parent, value, 0);
    result->node = *inserted_node;
    result->inserted = true;
    return result;
}
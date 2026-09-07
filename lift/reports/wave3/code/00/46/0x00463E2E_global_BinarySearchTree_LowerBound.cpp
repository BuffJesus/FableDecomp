#include <cstddef>
#include <cstdint>

struct RBTreeNodeOverlay {
    std::uint8_t pad_00[0x08];
    RBTreeNodeOverlay* left;
    RBTreeNodeOverlay* right;
    int key;
};

static_assert(offsetof(RBTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, key) == 0x10);

struct RBTreeOverlay {
    std::uint8_t pad_00[0x04];
    RBTreeNodeOverlay* root;
    RBTreeNodeOverlay* sentinel;
};

static_assert(offsetof(RBTreeOverlay, root) == 0x04);
static_assert(offsetof(RBTreeOverlay, sentinel) == 0x08);

struct BinarySearchTreeOverlay {
    RBTreeOverlay* tree;
};

static_assert(offsetof(BinarySearchTreeOverlay, tree) == 0x00);

struct LowerBoundResultOverlay {
    RBTreeNodeOverlay* node;
    std::uint8_t inserted;
};

static_assert(offsetof(LowerBoundResultOverlay, node) == 0x00);
static_assert(offsetof(LowerBoundResultOverlay, inserted) == 0x04);

extern RBTreeNodeOverlay* __cdecl RBTree__GetMaxNode(RBTreeNodeOverlay* node);
extern RBTreeNodeOverlay** __thiscall RBTree_Insert_Internal(
    void* self,
    int** key_ptr,
    RBTreeNodeOverlay* parent,
    RBTreeNodeOverlay* hint,
    int* key,
    int zero
);

LowerBoundResultOverlay* __thiscall BinarySearchTree_LowerBound(
    BinarySearchTreeOverlay* self,
    LowerBoundResultOverlay* result,
    int* key
) {
    RBTreeOverlay* const tree = self->tree;
    RBTreeNodeOverlay* current = tree->root;
    bool went_left = true;
    RBTreeNodeOverlay* last = reinterpret_cast<RBTreeNodeOverlay*>(tree);

    if (current != nullptr) {
        do {
            last = current;
            went_left = *key < last->key;
            if (went_left) {
                current = last->left;
            } else {
                current = last->right;
            }
        } while (current != nullptr);
    }

    RBTreeNodeOverlay* candidate = last;
    RBTreeNodeOverlay* insert_parent = current;

    if (went_left) {
        RBTreeNodeOverlay* const temp_parent = last;
        if (last == tree->sentinel) {
            RBTreeNodeOverlay** const inserted =
                RBTree_Insert_Internal(self, &key, temp_parent, last, key, 0);
            result->node = *inserted;
            result->inserted = 1;
            return result;
        }
        candidate = RBTree__GetMaxNode(last);
    }

    insert_parent = current;
    if (*key <= candidate->key) {
        result->node = candidate;
        result->inserted = 0;
        return result;
    }

    RBTreeNodeOverlay** const inserted =
        RBTree_Insert_Internal(self, &key, insert_parent, last, key, 0);
    result->node = *inserted;
    result->inserted = 1;
    return result;
}
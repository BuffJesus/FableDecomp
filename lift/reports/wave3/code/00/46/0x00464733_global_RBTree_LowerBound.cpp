#include <cstddef>

struct RBTreeNodeOverlay {
    char pad00[0x08];
    RBTreeNodeOverlay* left;   // 0x08
    RBTreeNodeOverlay* right;  // 0x0C
    int key;                   // 0x10
};

static_assert(offsetof(RBTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, key) == 0x10);

struct RBTreeHeaderOverlay {
    char pad00[0x04];
    RBTreeNodeOverlay* root;      // 0x04
    RBTreeNodeOverlay* nil_node;  // 0x08
};

static_assert(offsetof(RBTreeHeaderOverlay, root) == 0x04);
static_assert(offsetof(RBTreeHeaderOverlay, nil_node) == 0x08);

struct RBTreeLowerBoundResult {
    RBTreeNodeOverlay* node; // 0x00
    bool inserted;           // 0x04
    char pad05[3];
};

static_assert(offsetof(RBTreeLowerBoundResult, node) == 0x00);
static_assert(offsetof(RBTreeLowerBoundResult, inserted) == 0x04);

extern "C" RBTreeNodeOverlay* __cdecl RBTree__GetMaxNode(RBTreeNodeOverlay* node);
extern "C" RBTreeNodeOverlay** __thiscall RBTree_Insert_Internal(
    void* self,
    int** transfer_binary_in,
    RBTreeNodeOverlay* insert_parent,
    RBTreeNodeOverlay* parent,
    int* key_ptr,
    int zero);

RBTreeLowerBoundResult* __thiscall RBTree_LowerBound(
    void* self,
    RBTreeLowerBoundResult* result,
    int* key_ptr)
{
    auto* const header = *reinterpret_cast<RBTreeHeaderOverlay**>(self);
    RBTreeNodeOverlay* current = header->root;
    bool went_left = true;
    RBTreeNodeOverlay* parent = reinterpret_cast<RBTreeNodeOverlay*>(header);

    if (current != nullptr) {
        do {
            parent = current;
            went_left = (*key_ptr < parent->key);
            if (went_left) {
                current = parent->left;
            } else {
                current = parent->right;
            }
        } while (current != nullptr);
    }

    RBTreeNodeOverlay* candidate = parent;
    RBTreeNodeOverlay* insert_parent;

    if (went_left) {
        insert_parent = parent;
        if (parent == header->nil_node) {
            goto do_insert;
        }
        candidate = RBTree__GetMaxNode(parent);
    }

    insert_parent = current;
    if (*key_ptr <= candidate->key) {
        result->node = candidate;
        result->inserted = false;
        return result;
    }

do_insert:
    RBTreeNodeOverlay** const inserted_slot =
        RBTree_Insert_Internal(self, &key_ptr, insert_parent, parent, key_ptr, 0);
    result->node = *inserted_slot;
    result->inserted = true;
    return result;
}
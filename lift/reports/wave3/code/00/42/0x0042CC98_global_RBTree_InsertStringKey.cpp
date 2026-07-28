#include <cstddef>
#include <functional>

struct CCharString;

struct RBTreeNodeOverlay {
    std::byte pad0[0x08];
    RBTreeNodeOverlay* left;   // 0x08
    RBTreeNodeOverlay* right;  // 0x0C
    CCharString key;           // 0x10
};

static_assert(offsetof(RBTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, key) == 0x10);

struct RBTreeHeaderOverlay {
    std::byte pad0[0x04];
    RBTreeNodeOverlay* root;     // 0x04
    RBTreeNodeOverlay* leftmost; // 0x08
};

static_assert(offsetof(RBTreeHeaderOverlay, root) == 0x04);
static_assert(offsetof(RBTreeHeaderOverlay, leftmost) == 0x08);

struct RBTreeOverlay {
    RBTreeHeaderOverlay* header;           // 0x00
    std::byte pad4[0x04];
    std::less<CCharString> key_less;       // 0x08
};

static_assert(offsetof(RBTreeOverlay, header) == 0x00);
static_assert(offsetof(RBTreeOverlay, key_less) == 0x08);

struct RBTreeInsertResultOverlay {
    RBTreeNodeOverlay* node; // 0x00
    bool inserted;           // 0x04
};

static_assert(offsetof(RBTreeInsertResultOverlay, node) == 0x00);
static_assert(offsetof(RBTreeInsertResultOverlay, inserted) == 0x04);

extern RBTreeNodeOverlay* RBTree__GetMaxNode(RBTreeNodeOverlay* node);
extern RBTreeNodeOverlay** BinaryTree_InsertCString(
    CCharString** key_ref,
    RBTreeNodeOverlay* parent_for_insert,
    RBTreeNodeOverlay* search_parent,
    CCharString* key,
    int zero
);

void __thiscall RBTree_InsertStringKey(
    RBTreeOverlay* self,
    RBTreeInsertResultOverlay* out_result,
    CCharString* key
) {
    RBTreeHeaderOverlay* const header = self->header;
    bool go_left = true;
    RBTreeNodeOverlay* parent = reinterpret_cast<RBTreeNodeOverlay*>(header);
    RBTreeNodeOverlay* current = header->root;

    while (current != nullptr) {
        go_left = self->key_less(key, &current->key);
        parent = current;
        if (go_left) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    RBTreeNodeOverlay* local_parent = parent;
    RBTreeNodeOverlay* insert_parent;

    if (!go_left) {
check_predecessor:
        go_left = self->key_less(&local_parent->key, key);
        insert_parent = current;
        if (!go_left) {
            out_result->inserted = false;
            out_result->node = local_parent;
            return;
        }
    } else {
        insert_parent = parent;
        if (parent != reinterpret_cast<RBTreeNodeOverlay*>(header->leftmost)) {
            local_parent = RBTree__GetMaxNode(parent);
            goto check_predecessor;
        }
    }

    RBTreeNodeOverlay** const inserted_slot =
        BinaryTree_InsertCString(&key, insert_parent, parent, key, 0);
    local_parent = *inserted_slot;
    out_result->inserted = true;
    out_result->node = local_parent;
}
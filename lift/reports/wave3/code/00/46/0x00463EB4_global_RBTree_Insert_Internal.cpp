#include <cstddef>
#include <cstdint>

struct RBTreeNodeOverlay {
    std::byte pad_00[0x04];
    RBTreeNodeOverlay* parent; // 0x04
    RBTreeNodeOverlay* left;   // 0x08
    RBTreeNodeOverlay* right;  // 0x0C
    std::int32_t key;          // 0x10
};

static_assert(offsetof(RBTreeNodeOverlay, parent) == 0x04);
static_assert(offsetof(RBTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, key) == 0x10);

struct RBTreeHeaderOverlay {
    std::byte pad_00[0x04];
    RBTreeNodeOverlay* root;      // 0x04
    RBTreeNodeOverlay* leftmost;  // 0x08
    RBTreeNodeOverlay* rightmost; // 0x0C
};

static_assert(offsetof(RBTreeHeaderOverlay, root) == 0x04);
static_assert(offsetof(RBTreeHeaderOverlay, leftmost) == 0x08);
static_assert(offsetof(RBTreeHeaderOverlay, rightmost) == 0x0C);

struct RBTreeOverlay {
    RBTreeHeaderOverlay* header; // 0x00
    std::uint32_t count;         // 0x04
};

static_assert(offsetof(RBTreeOverlay, header) == 0x00);
static_assert(offsetof(RBTreeOverlay, count) == 0x04);

extern RBTreeNodeOverlay* __cdecl TreeNode_AllocData(std::int32_t* key);
extern void __cdecl CMemoryAllocatorVariableSize__BalanceTree(
    RBTreeNodeOverlay* node,
    void* root_link);

void __thiscall RBTree_Insert_Internal(
    RBTreeOverlay* self,
    RBTreeNodeOverlay** out_node,
    int param_3,
    RBTreeNodeOverlay* parent,
    std::int32_t* key,
    int param_6)
{
    RBTreeNodeOverlay* new_node;
    RBTreeHeaderOverlay* header;

    if ((parent == reinterpret_cast<RBTreeNodeOverlay*>(self->header)) ||
        ((param_6 == 0) && ((param_3 != 0) || (*key < parent->key)))) {
        new_node = TreeNode_AllocData(key);
        parent->left = new_node;

        header = self->header;
        if (parent != reinterpret_cast<RBTreeNodeOverlay*>(header)) {
            if (parent == header->leftmost) {
                header->leftmost = new_node;
            }
            goto finish_insert;
        }

        header->root = new_node;
        header = self->header;
    } else {
        new_node = TreeNode_AllocData(key);
        parent->right = new_node;

        header = self->header;
        if (parent != header->rightmost) {
            goto finish_insert;
        }
    }

    header->rightmost = new_node;

finish_insert:
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->parent = parent;

    CMemoryAllocatorVariableSize__BalanceTree(
        new_node,
        reinterpret_cast<std::byte*>(self->header) + 0x04);

    ++self->count;
    *out_node = new_node;
}
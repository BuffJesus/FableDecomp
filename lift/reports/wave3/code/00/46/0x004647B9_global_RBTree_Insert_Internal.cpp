#include <cstddef>
#include <cstdint>

struct RBTreeNodeOverlay {
    std::uint32_t color_or_flags;   // 0x00
    RBTreeNodeOverlay* parent;      // 0x04
    RBTreeNodeOverlay* left;        // 0x08
    RBTreeNodeOverlay* right;       // 0x0C
    std::int32_t key;               // 0x10
};

static_assert(offsetof(RBTreeNodeOverlay, parent) == 0x04);
static_assert(offsetof(RBTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, key) == 0x10);

struct RBTreeOverlay {
    RBTreeNodeOverlay* header;      // 0x00
    std::int32_t count;             // 0x04
};

static_assert(offsetof(RBTreeOverlay, header) == 0x00);
static_assert(offsetof(RBTreeOverlay, count) == 0x04);

extern RBTreeNodeOverlay* __thiscall TreeNode_AllocData(RBTreeOverlay* self, std::int32_t* data);
extern void __cdecl CMemoryAllocatorVariableSize__BalanceTree(
    RBTreeNodeOverlay* node,
    void* root_link_at_header_plus_4);

void __thiscall RBTree_Insert_Internal(
    RBTreeOverlay* self,
    RBTreeNodeOverlay** out_node,
    int param_3,
    RBTreeNodeOverlay* parent,
    std::int32_t* data,
    int param_6)
{
    RBTreeNodeOverlay* new_node;
    RBTreeNodeOverlay* header;

    if ((parent == self->header) ||
        ((param_6 == 0) && ((param_3 != 0) || (*data < parent->key)))) {
        new_node = TreeNode_AllocData(self, data);
        parent->left = new_node;
        header = self->header;

        if (parent != header) {
            if (parent == header->left) {
                header->left = new_node;
            }
            goto finish_link;
        }

        header->parent = new_node;
        header = self->header;
    } else {
        new_node = TreeNode_AllocData(self, data);
        parent->right = new_node;
        header = self->header;

        if (parent != header->right) {
            goto finish_link;
        }
    }

    header->right = new_node;

finish_link:
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->parent = parent;
    CMemoryAllocatorVariableSize__BalanceTree(
        new_node,
        reinterpret_cast<std::uint8_t*>(self->header) + 0x04);
    self->count = self->count + 1;
    *out_node = new_node;
}
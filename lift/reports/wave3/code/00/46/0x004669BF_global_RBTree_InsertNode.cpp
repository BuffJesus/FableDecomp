#include <cstddef>
#include <cstdint>

struct RBTreeNodeBaseOverlay {
    std::uint32_t color_or_flags;      // 0x00
    RBTreeNodeBaseOverlay* parent;     // 0x04
    RBTreeNodeBaseOverlay* left;       // 0x08
    RBTreeNodeBaseOverlay* right;      // 0x0C
};

static_assert(offsetof(RBTreeNodeBaseOverlay, parent) == 0x04);
static_assert(offsetof(RBTreeNodeBaseOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeBaseOverlay, right) == 0x0C);

struct RBTreeNodeOverlay {
    RBTreeNodeBaseOverlay base;        // 0x00
    int key;                           // 0x10
};

static_assert(offsetof(RBTreeNodeOverlay, key) == 0x10);

struct RBTreeOverlay {
    RBTreeNodeBaseOverlay* header;     // 0x00
    int node_count;                    // 0x04
};

static_assert(offsetof(RBTreeOverlay, header) == 0x00);
static_assert(offsetof(RBTreeOverlay, node_count) == 0x04);

extern RBTreeNodeBaseOverlay* __thiscall TreeNode_AllocData(
    RBTreeOverlay* tree,
    int* source_data);

extern "C" void __cdecl CMemoryAllocatorVariableSize__BalanceTree(
    RBTreeNodeBaseOverlay* node,
    RBTreeNodeBaseOverlay** root_link);

void __thiscall RBTree_InsertNode(
    RBTreeOverlay* tree,
    RBTreeNodeBaseOverlay** out_node,
    int param_3,
    RBTreeNodeBaseOverlay* parent,
    int* source_data,
    int param_6)
{
    RBTreeNodeBaseOverlay* new_node;
    RBTreeNodeBaseOverlay* header;

    if ((parent == tree->header) ||
        ((param_6 == 0) &&
         ((param_3 != 0) || (*source_data < reinterpret_cast<RBTreeNodeOverlay*>(parent)->key)))) {
        new_node = TreeNode_AllocData(tree, source_data);
        parent->left = new_node;

        header = tree->header;
        if (parent != header) {
            if (parent == header->left) {
                header->left = new_node;
            }
            goto link_done;
        }

        header->parent = new_node;
        header = tree->header;
    } else {
        new_node = TreeNode_AllocData(tree, source_data);
        parent->right = new_node;

        header = tree->header;
        if (parent != header->right) {
            goto link_done;
        }
    }

    header->right = new_node;

link_done:
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->parent = parent;
    CMemoryAllocatorVariableSize__BalanceTree(new_node, &tree->header->parent);
    tree->node_count = tree->node_count + 1;
    *out_node = new_node;
}
#include <cstddef>
#include <cstdint>

struct BinaryTreeNodeOverlay {
    std::uint8_t pad0[0x4];
    BinaryTreeNodeOverlay* parent; // 0x04
    BinaryTreeNodeOverlay* left;   // 0x08
    BinaryTreeNodeOverlay* right;  // 0x0C
    std::int32_t key;              // 0x10
};

static_assert(offsetof(BinaryTreeNodeOverlay, parent) == 0x04);
static_assert(offsetof(BinaryTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(BinaryTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(BinaryTreeNodeOverlay, key) == 0x10);

struct BinaryTreeOverlay {
    BinaryTreeNodeOverlay* header; // 0x00
    std::int32_t count;            // 0x04
};

static_assert(offsetof(BinaryTreeOverlay, header) == 0x00);
static_assert(offsetof(BinaryTreeOverlay, count) == 0x04);

extern "C" BinaryTreeNodeOverlay* __thiscall
BinarySearchTreeNode_CreateDuplicate(BinaryTreeOverlay* self, const void* source);

extern "C" void
CMemoryAllocatorVariableSize__BalanceTree(BinaryTreeNodeOverlay* node, void* root_link);

void __thiscall BinaryTree_InsertNodeLeft(
    BinaryTreeOverlay* self,
    BinaryTreeNodeOverlay** out_inserted,
    int param_3,
    BinaryTreeNodeOverlay* param_4,
    const std::int32_t* param_5,
    int param_6)
{
    BinaryTreeNodeOverlay* new_node;
    BinaryTreeNodeOverlay* header;

    if ((param_4 == self->header) ||
        ((param_6 == 0) && ((param_3 != 0) || ((*param_5) < param_4->key)))) {
        new_node = BinarySearchTreeNode_CreateDuplicate(self, param_5);
        param_4->left = new_node;
        header = self->header;
        if (param_4 != header) {
            if (param_4 == header->left) {
                header->left = new_node;
            }
            goto link_done;
        }
        header->parent = new_node;
        header = self->header;
    } else {
        new_node = BinarySearchTreeNode_CreateDuplicate(self, param_5);
        param_4->right = new_node;
        header = self->header;
        if (param_4 != header->right) {
            goto link_done;
        }
    }

    header->right = new_node;

link_done:
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->parent = param_4;
    CMemoryAllocatorVariableSize__BalanceTree(
        new_node,
        reinterpret_cast<std::uint8_t*>(self->header) + 0x4);
    self->count = self->count + 1;
    *out_inserted = new_node;
}
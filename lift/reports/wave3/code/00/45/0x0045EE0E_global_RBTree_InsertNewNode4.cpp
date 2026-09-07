#include <cstddef>
#include <cstdint>

struct RBTreeNode4Overlay {
    std::uint8_t pad_00[0x04];
    RBTreeNode4Overlay* parent; // 0x04
    RBTreeNode4Overlay* left;   // 0x08
    RBTreeNode4Overlay* right;  // 0x0C
    int value;                  // 0x10
};

static_assert(offsetof(RBTreeNode4Overlay, parent) == 0x04);
static_assert(offsetof(RBTreeNode4Overlay, left) == 0x08);
static_assert(offsetof(RBTreeNode4Overlay, right) == 0x0C);
static_assert(offsetof(RBTreeNode4Overlay, value) == 0x10);

struct RBTree4Overlay {
    RBTreeNode4Overlay* header; // 0x00
    int nodeCount;              // 0x04
};

static_assert(offsetof(RBTree4Overlay, header) == 0x00);
static_assert(offsetof(RBTree4Overlay, nodeCount) == 0x04);

extern RBTreeNode4Overlay* TreeNode_AllocData_Byte(int* value);
extern void CMemoryAllocatorVariableSize__BalanceTree(
    RBTreeNode4Overlay* node,
    void* root_link_base);

void __thiscall RBTree_InsertNewNode4(
    RBTree4Overlay* this_,
    RBTreeNode4Overlay** insertedNode,
    int param_3,
    RBTreeNode4Overlay* parent,
    int* value,
    int param_6)
{
    RBTreeNode4Overlay* newNode;
    RBTreeNode4Overlay* header;

    if ((parent == this_->header) ||
        ((param_6 == 0) && ((param_3 != 0) || (*value < parent->value)))) {
        newNode = TreeNode_AllocData_Byte(value);
        parent->left = newNode;

        header = this_->header;
        if (parent != header) {
            if (parent == header->left) {
                header->left = newNode;
            }
            goto link_complete;
        }

        header->parent = newNode;
        header = this_->header;
    } else {
        newNode = TreeNode_AllocData_Byte(value);
        parent->right = newNode;

        header = this_->header;
        if (parent != header->right) {
            goto link_complete;
        }
    }

    header->right = newNode;

link_complete:
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->parent = parent;
    CMemoryAllocatorVariableSize__BalanceTree(
        newNode,
        reinterpret_cast<std::uint8_t*>(this_->header) + 0x04);
    this_->nodeCount = this_->nodeCount + 1;
    *insertedNode = newNode;
}
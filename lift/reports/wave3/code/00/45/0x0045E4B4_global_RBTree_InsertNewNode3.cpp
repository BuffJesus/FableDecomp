#include <cstddef>
#include <cstdint>

struct RBTreeNode3Overlay {
    std::uint32_t unknown0;
    RBTreeNode3Overlay* parent; // 0x04
    RBTreeNode3Overlay* left;   // 0x08
    RBTreeNode3Overlay* right;  // 0x0C
    std::int32_t value;         // 0x10
};

static_assert(offsetof(RBTreeNode3Overlay, parent) == 0x04);
static_assert(offsetof(RBTreeNode3Overlay, left) == 0x08);
static_assert(offsetof(RBTreeNode3Overlay, right) == 0x0C);
static_assert(offsetof(RBTreeNode3Overlay, value) == 0x10);

struct RBTreeState3Overlay {
    RBTreeNode3Overlay* header; // 0x00
    std::int32_t size;          // 0x04
};

static_assert(offsetof(RBTreeState3Overlay, header) == 0x00);
static_assert(offsetof(RBTreeState3Overlay, size) == 0x04);

extern RBTreeNode3Overlay* TreeNode_AllocData_Byte(const std::int32_t* value);
extern void CMemoryAllocatorVariableSize__BalanceTree(
    RBTreeNode3Overlay* node,
    void* headerParentLink);

void __thiscall RBTree_InsertNewNode3(
    RBTreeState3Overlay* tree,
    RBTreeNode3Overlay** insertedNodeOut,
    int param_3,
    RBTreeNode3Overlay* parentNode,
    std::int32_t* value,
    int param_6)
{
    RBTreeNode3Overlay* newNode;
    RBTreeNode3Overlay* header;

    if ((parentNode == tree->header) ||
        ((param_6 == 0) &&
         ((param_3 != 0) || (*value < parentNode->value)))) {
        newNode = TreeNode_AllocData_Byte(value);
        parentNode->left = newNode;
        header = tree->header;

        if (parentNode != header) {
            if (parentNode == header->left) {
                header->left = newNode;
            }
            goto finish_link;
        }

        header->parent = newNode;
        header = tree->header;
    } else {
        newNode = TreeNode_AllocData_Byte(value);
        parentNode->right = newNode;
        header = tree->header;

        if (parentNode != header->right) {
            goto finish_link;
        }
    }

    header->right = newNode;

finish_link:
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->parent = parentNode;
    CMemoryAllocatorVariableSize__BalanceTree(
        newNode,
        reinterpret_cast<std::byte*>(tree->header) + 0x04);
    tree->size = tree->size + 1;
    *insertedNodeOut = newNode;
}
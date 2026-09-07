#include <cstddef>
#include <cstdint>

struct BinarySearchTreeNodeOverlay {
    std::byte pad_0x00[0x4];
    BinarySearchTreeNodeOverlay* parent; // 0x04
    BinarySearchTreeNodeOverlay* left;   // 0x08
    BinarySearchTreeNodeOverlay* right;  // 0x0C
    std::uint32_t key;                   // 0x10
};

static_assert(offsetof(BinarySearchTreeNodeOverlay, parent) == 0x04);
static_assert(offsetof(BinarySearchTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(BinarySearchTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(BinarySearchTreeNodeOverlay, key) == 0x10);

struct BinarySearchTreeOverlay {
    BinarySearchTreeNodeOverlay* header; // 0x00
    std::int32_t nodeCount;              // 0x04
};

static_assert(offsetof(BinarySearchTreeOverlay, header) == 0x00);
static_assert(offsetof(BinarySearchTreeOverlay, nodeCount) == 0x04);

BinarySearchTreeNodeOverlay* __thiscall TreeNode_AllocData(
    BinarySearchTreeOverlay* tree,
    const std::uint32_t* value);

void __cdecl CMemoryAllocatorVariableSize__BalanceTree(
    BinarySearchTreeNodeOverlay* node,
    BinarySearchTreeNodeOverlay** rootLink);

void __thiscall BinarySearchTree_InsertNode(
    BinarySearchTreeOverlay* tree,
    BinarySearchTreeNodeOverlay** outNode,
    int param_3,
    BinarySearchTreeNodeOverlay* parentNode,
    const std::uint32_t* value,
    int param_6)
{
    BinarySearchTreeNodeOverlay* newNode;
    BinarySearchTreeNodeOverlay* header;

    if ((parentNode == tree->header) ||
        ((param_6 == 0) && ((param_3 != 0) || (*value < parentNode->key)))) {
        newNode = TreeNode_AllocData(tree, value);
        parentNode->left = newNode;

        header = tree->header;
        if (parentNode != header) {
            if (parentNode == header->left) {
                header->left = newNode;
            }
            goto link_done;
        }

        header->parent = newNode;
        header = tree->header;
    } else {
        newNode = TreeNode_AllocData(tree, value);
        parentNode->right = newNode;

        header = tree->header;
        if (parentNode != header->right) {
            goto link_done;
        }
    }

    header->right = newNode;

link_done:
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->parent = parentNode;
    CMemoryAllocatorVariableSize__BalanceTree(newNode, &tree->header->parent);
    tree->nodeCount = tree->nodeCount + 1;
    *outNode = newNode;
}
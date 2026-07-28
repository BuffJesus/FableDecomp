#include <cstddef>
#include <cstdint>

struct BinarySearchTreeNodeOverlay {
    std::uint32_t reserved0;                    // 0x00
    BinarySearchTreeNodeOverlay* parent;        // 0x04
    BinarySearchTreeNodeOverlay* left;          // 0x08
    BinarySearchTreeNodeOverlay* right;         // 0x0C
    std::uint32_t key;                          // 0x10
};

static_assert(offsetof(BinarySearchTreeNodeOverlay, parent) == 0x04);
static_assert(offsetof(BinarySearchTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(BinarySearchTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(BinarySearchTreeNodeOverlay, key) == 0x10);

struct BinarySearchTreeOverlay {
    BinarySearchTreeNodeOverlay* header;        // 0x00
    int count;                                  // 0x04
};

static_assert(offsetof(BinarySearchTreeOverlay, header) == 0x00);
static_assert(offsetof(BinarySearchTreeOverlay, count) == 0x04);

extern BinarySearchTreeNodeOverlay* BinarySearchTreeNode_Create(std::uint32_t* value);
extern void CMemoryAllocatorVariableSize__BalanceTree(
    BinarySearchTreeNodeOverlay* node,
    void* rootLink);

void __thiscall BinarySearchTree_InsertAndBalance(
    BinarySearchTreeOverlay* self,
    BinarySearchTreeNodeOverlay** insertedNode,
    int param_3,
    BinarySearchTreeNodeOverlay* parentOrHeader,
    std::uint32_t* value,
    int param_6)
{
    BinarySearchTreeNodeOverlay* newNode;
    BinarySearchTreeNodeOverlay* header;

    if ((parentOrHeader == self->header) ||
        ((param_6 == 0) && ((param_3 != 0) || (*value < parentOrHeader->key)))) {
        newNode = BinarySearchTreeNode_Create(value);
        parentOrHeader->left = newNode;

        header = self->header;
        if (parentOrHeader != header) {
            if (parentOrHeader == header->left) {
                header->left = newNode;
            }
            goto finish_link;
        }

        header->parent = newNode;
        header = self->header;
    } else {
        newNode = BinarySearchTreeNode_Create(value);
        parentOrHeader->right = newNode;

        header = self->header;
        if (parentOrHeader != header->right) {
            goto finish_link;
        }
    }

    header->right = newNode;

finish_link:
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->parent = parentOrHeader;
    CMemoryAllocatorVariableSize__BalanceTree(
        newNode,
        reinterpret_cast<std::uint8_t*>(self->header) + 4);
    self->count = self->count + 1;
    *insertedNode = newNode;
}
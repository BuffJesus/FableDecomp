#include <cstddef>
#include <cstdint>

struct RBTreeNodeOverlay {
    std::uint32_t unknown_0;
    RBTreeNodeOverlay* parent_or_root;          // +0x04: parent for a node, root for the header sentinel
    RBTreeNodeOverlay* left_or_leftmost;        // +0x08: left child for a node, leftmost for the header sentinel
    RBTreeNodeOverlay* right_or_rightmost;      // +0x0C: right child for a node, rightmost for the header sentinel
    std::int32_t key;                           // +0x10
};

static_assert(offsetof(RBTreeNodeOverlay, parent_or_root) == 0x04);
static_assert(offsetof(RBTreeNodeOverlay, left_or_leftmost) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right_or_rightmost) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, key) == 0x10);

struct RBTreeOverlay {
    RBTreeNodeOverlay* header;
    std::int32_t size;
};

static_assert(offsetof(RBTreeOverlay, header) == 0x00);
static_assert(offsetof(RBTreeOverlay, size) == 0x04);

extern "C" RBTreeNodeOverlay* __thiscall TreeNode_AllocData(RBTreeOverlay* self, int* valueSource);
extern "C" void __cdecl CMemoryAllocatorVariableSize__BalanceTree(RBTreeNodeOverlay* node, void* rootSlot);

void __thiscall RBTree_InsertNewNode5(
    RBTreeOverlay* self,
    RBTreeNodeOverlay** insertedNodeOut,
    int flag3,
    RBTreeNodeOverlay* parentNode,
    int* valueSource,
    int flag6)
{
    RBTreeNodeOverlay* newNode;
    RBTreeNodeOverlay* header;

    if (parentNode == self->header ||
        (flag6 == 0 && (flag3 != 0 || *valueSource < parentNode->key))) {
        newNode = TreeNode_AllocData(self, valueSource);
        parentNode->left_or_leftmost = newNode;

        header = self->header;
        if (parentNode != header) {
            if (parentNode == header->left_or_leftmost) {
                header->left_or_leftmost = newNode;
            }
            goto link_complete;
        }

        header->parent_or_root = newNode;
        header = self->header;
    } else {
        newNode = TreeNode_AllocData(self, valueSource);
        parentNode->right_or_rightmost = newNode;

        header = self->header;
        if (parentNode != header->right_or_rightmost) {
            goto link_complete;
        }
    }

    header->right_or_rightmost = newNode;

link_complete:
    newNode->left_or_leftmost = nullptr;
    newNode->right_or_rightmost = nullptr;
    newNode->parent_or_root = parentNode;

    CMemoryAllocatorVariableSize__BalanceTree(
        newNode,
        reinterpret_cast<std::uint8_t*>(self->header) + 0x04);

    self->size = self->size + 1;
    *insertedNodeOut = newNode;
}
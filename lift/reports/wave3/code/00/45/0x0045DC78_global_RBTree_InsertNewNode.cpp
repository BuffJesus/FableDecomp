#include <cstddef>
#include <cstdint>

struct RBTreeNodeOverlay
{
    std::uint32_t unknown0;
    RBTreeNodeOverlay* parent;
    RBTreeNodeOverlay* left;
    RBTreeNodeOverlay* right;
    std::int32_t key;
};

static_assert(offsetof(RBTreeNodeOverlay, parent) == 0x04);
static_assert(offsetof(RBTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, key) == 0x10);

struct RBTreeOverlay
{
    RBTreeNodeOverlay* head;
    std::int32_t count;
};

static_assert(offsetof(RBTreeOverlay, head) == 0x00);
static_assert(offsetof(RBTreeOverlay, count) == 0x04);

extern "C" RBTreeNodeOverlay* __thiscall TreeNode_AllocData(void* self, int* data);
extern "C" void __cdecl CMemoryAllocatorVariableSize__BalanceTree(
    RBTreeNodeOverlay* node,
    RBTreeNodeOverlay** rootLink);

void __thiscall RBTree_InsertNewNode(
    RBTreeOverlay* self,
    RBTreeNodeOverlay** outNode,
    int param_3,
    RBTreeNodeOverlay* parent,
    int* data,
    int param_6)
{
    RBTreeNodeOverlay* newNode;
    RBTreeNodeOverlay* head;

    if ((parent == self->head) ||
        ((param_6 == 0) && ((param_3 != 0) || (*data < parent->key))))
    {
        newNode = TreeNode_AllocData(self, data);
        parent->left = newNode;
        head = self->head;
        if (parent != head)
        {
            if (parent == head->left)
            {
                head->left = newNode;
            }
            goto link_done;
        }

        head->parent = newNode;
        head = self->head;
    }
    else
    {
        newNode = TreeNode_AllocData(self, data);
        parent->right = newNode;
        head = self->head;
        if (parent != head->right)
        {
            goto link_done;
        }

        head->right = newNode;
    }

link_done:
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->parent = parent;
    CMemoryAllocatorVariableSize__BalanceTree(newNode, &self->head->parent);
    self->count = self->count + 1;
    *outNode = newNode;
}
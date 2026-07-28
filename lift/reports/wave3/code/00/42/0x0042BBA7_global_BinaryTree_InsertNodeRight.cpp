#include <cstddef>
#include <cstdint>

struct BinaryTreeNodeOverlay
{
    std::uint32_t unk0;
    BinaryTreeNodeOverlay* parent;
    BinaryTreeNodeOverlay* left;
    BinaryTreeNodeOverlay* right;
    std::int32_t key;
};

static_assert(offsetof(BinaryTreeNodeOverlay, parent) == 0x04);
static_assert(offsetof(BinaryTreeNodeOverlay, left)   == 0x08);
static_assert(offsetof(BinaryTreeNodeOverlay, right)  == 0x0C);
static_assert(offsetof(BinaryTreeNodeOverlay, key)    == 0x10);

struct BinaryTreeOverlay
{
    BinaryTreeNodeOverlay* head;
    std::int32_t count;
};

static_assert(offsetof(BinaryTreeOverlay, head)  == 0x00);
static_assert(offsetof(BinaryTreeOverlay, count) == 0x04);

extern "C" BinaryTreeNodeOverlay* __thiscall BinarySearchTreeNode_CreateCopy(const std::int32_t* source);
extern "C" void __cdecl CMemoryAllocatorVariableSize__BalanceTree(
    BinaryTreeNodeOverlay* node,
    void* rebalanceRootLink);

void __thiscall ::BinaryTree_InsertNodeRight(
    BinaryTreeOverlay* self,
    BinaryTreeNodeOverlay** outInsertedNode,
    int param_3,
    BinaryTreeNodeOverlay* parentNode,
    const std::int32_t* source,
    int param_6)
{
    BinaryTreeNodeOverlay* newNode;
    BinaryTreeNodeOverlay* head = self->head;

    if ((parentNode == head) ||
        ((param_6 == 0) && ((param_3 != 0) || (*source < parentNode->key))))
    {
        newNode = BinarySearchTreeNode_CreateCopy(source);
        parentNode->left = newNode;

        head = self->head;
        if (parentNode != head)
        {
            if (parentNode == head->left)
            {
                head->left = newNode;
            }
            goto finish_link;
        }

        head->parent = newNode;
        head = self->head;
    }
    else
    {
        newNode = BinarySearchTreeNode_CreateCopy(source);
        parentNode->right = newNode;

        head = self->head;
        if (parentNode != head->right)
        {
            goto finish_link;
        }
    }

    head->right = newNode;

finish_link:
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->parent = parentNode;
    CMemoryAllocatorVariableSize__BalanceTree(
        newNode,
        reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(self->head) + 0x04));
    self->count = self->count + 1;
    *outInsertedNode = newNode;
}
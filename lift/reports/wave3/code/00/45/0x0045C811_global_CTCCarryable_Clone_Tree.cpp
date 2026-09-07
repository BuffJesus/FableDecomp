#include <cstddef>
#include <cstdint>

struct TCCarryableTreeNode
{
    std::uint8_t type;
    std::uint8_t pad_01[3];
    TCCarryableTreeNode* parent;
    TCCarryableTreeNode* sibling;
    TCCarryableTreeNode* child;
    std::byte payload[1];
};

static_assert(offsetof(TCCarryableTreeNode, parent) == 0x04);
static_assert(offsetof(TCCarryableTreeNode, sibling) == 0x08);
static_assert(offsetof(TCCarryableTreeNode, child) == 0x0C);
static_assert(offsetof(TCCarryableTreeNode, payload) == 0x10);

extern "C" TCCarryableTreeNode* TreeNode_AllocData(const void* sourcePayload);

TCCarryableTreeNode* __stdcall CTCCarryable_Clone_Tree(
    TCCarryableTreeNode* source,
    TCCarryableTreeNode* parent)
{
    TCCarryableTreeNode* rootClone =
        TreeNode_AllocData(static_cast<const void*>(source->payload));

    rootClone->sibling = nullptr;
    rootClone->child = nullptr;
    rootClone->type = source->type;
    rootClone->parent = parent;

    if (source->child != nullptr)
    {
        rootClone->child = CTCCarryable_Clone_Tree(source->child, rootClone);
    }

    parent = rootClone;
    for (TCCarryableTreeNode* sourceSibling = source->sibling;
         sourceSibling != nullptr;
         sourceSibling = sourceSibling->sibling)
    {
        TCCarryableTreeNode* siblingClone =
            TreeNode_AllocData(static_cast<const void*>(sourceSibling->payload));

        siblingClone->sibling = nullptr;
        siblingClone->child = nullptr;
        siblingClone->type = sourceSibling->type;
        parent->sibling = siblingClone;
        siblingClone->parent = parent;

        if (sourceSibling->child != nullptr)
        {
            siblingClone->child = CTCCarryable_Clone_Tree(sourceSibling->child, siblingClone);
        }

        parent = siblingClone;
    }

    return rootClone;
}
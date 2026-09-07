#include <cstddef>
#include <cstdint>

struct TreeNode {
    std::uint8_t nodeType;
    std::byte pad_01[3];
    TreeNode* parent;
    TreeNode* nextSibling;
    TreeNode* firstChild;
    std::byte payload[1];
};

static_assert(offsetof(TreeNode, parent) == 0x04);
static_assert(offsetof(TreeNode, nextSibling) == 0x08);
static_assert(offsetof(TreeNode, firstChild) == 0x0C);
static_assert(offsetof(TreeNode, payload) == 0x10);

extern TreeNode* TreeNode_AllocData(const void* sourcePayload);

TreeNode* TreeNode_CloneDeep(TreeNode* source, TreeNode* parent)
{
    TreeNode* clonedRoot = TreeNode_AllocData(&source->payload[0]);
    const std::uint8_t nodeType = source->nodeType;

    clonedRoot->nextSibling = nullptr;
    clonedRoot->firstChild = nullptr;
    clonedRoot->nodeType = nodeType;
    clonedRoot->parent = parent;

    if (source->firstChild != nullptr) {
        clonedRoot->firstChild = TreeNode_CloneDeep(source->firstChild, clonedRoot);
    }

    TreeNode* tail = clonedRoot;
    for (TreeNode* sibling = source->nextSibling; sibling != nullptr; sibling = sibling->nextSibling) {
        TreeNode* clonedSibling = TreeNode_AllocData(&sibling->payload[0]);
        const std::uint8_t siblingType = sibling->nodeType;

        clonedSibling->nextSibling = nullptr;
        clonedSibling->firstChild = nullptr;
        clonedSibling->nodeType = siblingType;
        tail->nextSibling = clonedSibling;
        clonedSibling->parent = tail;

        if (sibling->firstChild != nullptr) {
            clonedSibling->firstChild = TreeNode_CloneDeep(sibling->firstChild, clonedSibling);
        }

        tail = clonedSibling;
    }

    return clonedRoot;
}
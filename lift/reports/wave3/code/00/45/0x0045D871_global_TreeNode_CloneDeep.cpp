#include <cstddef>
#include <cstdint>

struct TreeNodeOverlay {
    std::uint8_t kind;
    std::uint8_t pad01[3];
    TreeNodeOverlay* prevOrParent;
    TreeNodeOverlay* nextSibling;
    TreeNodeOverlay* firstChild;
    std::uint8_t payload[1];
};

static_assert(offsetof(TreeNodeOverlay, kind) == 0x00);
static_assert(offsetof(TreeNodeOverlay, prevOrParent) == 0x04);
static_assert(offsetof(TreeNodeOverlay, nextSibling) == 0x08);
static_assert(offsetof(TreeNodeOverlay, firstChild) == 0x0C);
static_assert(offsetof(TreeNodeOverlay, payload) == 0x10);

extern "C" TreeNodeOverlay* TreeNode_AllocData(const void* sourcePayload);

extern "C" TreeNodeOverlay* __stdcall TreeNode_CloneDeep(TreeNodeOverlay* source, TreeNodeOverlay* link)
{
    TreeNodeOverlay* const clonedRoot = TreeNode_AllocData(source->payload);
    clonedRoot->nextSibling = nullptr;
    clonedRoot->firstChild = nullptr;
    clonedRoot->kind = source->kind;
    clonedRoot->prevOrParent = link;

    if (source->firstChild != nullptr) {
        clonedRoot->firstChild = TreeNode_CloneDeep(source->firstChild, clonedRoot);
    }

    TreeNodeOverlay* tail = clonedRoot;
    for (TreeNodeOverlay* sibling = source->nextSibling; sibling != nullptr; sibling = sibling->nextSibling) {
        TreeNodeOverlay* const clonedSibling = TreeNode_AllocData(sibling->payload);
        clonedSibling->nextSibling = nullptr;
        clonedSibling->firstChild = nullptr;
        clonedSibling->kind = sibling->kind;
        tail->nextSibling = clonedSibling;
        clonedSibling->prevOrParent = tail;

        if (sibling->firstChild != nullptr) {
            clonedSibling->firstChild = TreeNode_CloneDeep(sibling->firstChild, clonedSibling);
        }

        tail = clonedSibling;
    }

    return clonedRoot;
}
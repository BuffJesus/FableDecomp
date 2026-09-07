#include <cstddef>
#include <cstdint>

struct TreeNodeOverlay {
    std::uint8_t type;
    std::uint8_t pad_01[3];
    TreeNodeOverlay* parentOrPrev;
    TreeNodeOverlay* nextSibling;
    TreeNodeOverlay* firstChild;
    std::uint8_t payload[1];
};

static_assert(offsetof(TreeNodeOverlay, type) == 0x00);
static_assert(offsetof(TreeNodeOverlay, parentOrPrev) == 0x04);
static_assert(offsetof(TreeNodeOverlay, nextSibling) == 0x08);
static_assert(offsetof(TreeNodeOverlay, firstChild) == 0x0C);
static_assert(offsetof(TreeNodeOverlay, payload) == 0x10);

extern "C" void* __cdecl TreeNode_AllocData_Byte(void* sourceData);

extern "C" TreeNodeOverlay* __stdcall TreeNode_CloneDeep(
    TreeNodeOverlay* source,
    TreeNodeOverlay* link)
{
    TreeNodeOverlay* const clonedRoot = static_cast<TreeNodeOverlay*>(
        TreeNode_AllocData_Byte(source->payload));

    clonedRoot->nextSibling = nullptr;
    clonedRoot->firstChild = nullptr;
    clonedRoot->type = source->type;
    clonedRoot->parentOrPrev = link;

    if (source->firstChild != nullptr) {
        clonedRoot->firstChild = TreeNode_CloneDeep(source->firstChild, clonedRoot);
    }

    TreeNodeOverlay* tail = clonedRoot;
    for (TreeNodeOverlay* sibling = source->nextSibling;
         sibling != nullptr;
         sibling = sibling->nextSibling) {
        TreeNodeOverlay* const clonedSibling = static_cast<TreeNodeOverlay*>(
            TreeNode_AllocData_Byte(sibling->payload));

        clonedSibling->nextSibling = nullptr;
        clonedSibling->firstChild = nullptr;
        clonedSibling->type = sibling->type;
        tail->nextSibling = clonedSibling;
        clonedSibling->parentOrPrev = tail;

        if (sibling->firstChild != nullptr) {
            clonedSibling->firstChild = TreeNode_CloneDeep(sibling->firstChild, clonedSibling);
        }

        tail = clonedSibling;
    }

    return clonedRoot;
}
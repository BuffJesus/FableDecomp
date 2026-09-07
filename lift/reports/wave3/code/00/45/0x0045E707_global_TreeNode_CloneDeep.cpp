#include <cstddef>
#include <cstdint>

struct TreeNodeOverlay {
    std::uint8_t type;
    std::uint8_t pad_01[3];
    TreeNodeOverlay* parent;
    TreeNodeOverlay* nextSibling;
    TreeNodeOverlay* firstChild;
};

static_assert(offsetof(TreeNodeOverlay, type) == 0x00);
static_assert(offsetof(TreeNodeOverlay, parent) == 0x04);
static_assert(offsetof(TreeNodeOverlay, nextSibling) == 0x08);
static_assert(offsetof(TreeNodeOverlay, firstChild) == 0x0C);
static_assert(sizeof(TreeNodeOverlay) == 0x10);

extern "C" void* TreeNode_AllocData_Byte(const void* sourceData);

TreeNodeOverlay* TreeNode_CloneDeep(TreeNodeOverlay* source, TreeNodeOverlay* parent)
{
    auto* clone = static_cast<TreeNodeOverlay*>(
        TreeNode_AllocData_Byte(reinterpret_cast<std::uint8_t*>(source) + 0x10));

    clone->type = source->type;
    clone->nextSibling = nullptr;
    clone->firstChild = nullptr;
    clone->parent = parent;

    if (source->firstChild != nullptr) {
        clone->firstChild = TreeNode_CloneDeep(source->firstChild, clone);
    }

    TreeNodeOverlay* tail = clone;
    for (TreeNodeOverlay* sibling = source->nextSibling; sibling != nullptr; sibling = sibling->nextSibling) {
        auto* siblingClone = static_cast<TreeNodeOverlay*>(
            TreeNode_AllocData_Byte(reinterpret_cast<std::uint8_t*>(sibling) + 0x10));

        siblingClone->type = sibling->type;
        siblingClone->nextSibling = nullptr;
        siblingClone->firstChild = nullptr;

        tail->nextSibling = siblingClone;
        siblingClone->parent = tail;

        if (sibling->firstChild != nullptr) {
            siblingClone->firstChild = TreeNode_CloneDeep(sibling->firstChild, siblingClone);
        }

        tail = siblingClone;
    }

    return clone;
}
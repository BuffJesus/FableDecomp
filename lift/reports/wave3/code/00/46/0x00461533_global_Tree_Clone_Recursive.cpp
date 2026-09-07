#include <cstddef>
#include <cstdint>

struct TreeNodeOverlay
{
    std::uint8_t type;       // 0x00
    std::byte pad01[3];      // 0x01
    TreeNodeOverlay* parent; // 0x04
    TreeNodeOverlay* sibling;// 0x08
    TreeNodeOverlay* child;  // 0x0C
    std::byte payload[1];    // 0x10
};

static_assert(offsetof(TreeNodeOverlay, type) == 0x00);
static_assert(offsetof(TreeNodeOverlay, parent) == 0x04);
static_assert(offsetof(TreeNodeOverlay, sibling) == 0x08);
static_assert(offsetof(TreeNodeOverlay, child) == 0x0C);
static_assert(offsetof(TreeNodeOverlay, payload) == 0x10);

extern "C" TreeNodeOverlay* __stdcall TreeNode_AllocData(const void* sourcePayload);

extern "C" TreeNodeOverlay* __stdcall Tree_Clone_Recursive(
    TreeNodeOverlay* source,
    TreeNodeOverlay* parent)
{
    TreeNodeOverlay* const rootClone = TreeNode_AllocData(source->payload);
    const std::uint8_t sourceType = source->type;

    rootClone->sibling = nullptr;
    rootClone->child = nullptr;
    rootClone->type = sourceType;
    rootClone->parent = parent;

    if (source->child != nullptr)
    {
        rootClone->child = Tree_Clone_Recursive(source->child, rootClone);
    }

    parent = rootClone;
    for (TreeNodeOverlay* sibling = source->sibling; sibling != nullptr; sibling = sibling->sibling)
    {
        TreeNodeOverlay* const siblingClone = TreeNode_AllocData(sibling->payload);
        const std::uint8_t siblingType = sibling->type;

        siblingClone->sibling = nullptr;
        siblingClone->child = nullptr;
        siblingClone->type = siblingType;
        parent->sibling = siblingClone;
        siblingClone->parent = parent;

        if (sibling->child != nullptr)
        {
            siblingClone->child = Tree_Clone_Recursive(sibling->child, siblingClone);
        }

        parent = siblingClone;
    }

    return rootClone;
}
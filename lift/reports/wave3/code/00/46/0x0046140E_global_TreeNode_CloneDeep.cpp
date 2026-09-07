#include <cstddef>
#include <cstdint>

struct TreeNodeOverlay
{
    std::uint8_t nodeType;   // 0x00
    std::byte pad01[3];      // 0x01
    TreeNodeOverlay* link04; // 0x04
    TreeNodeOverlay* next08; // 0x08
    TreeNodeOverlay* child0C;// 0x0C
    std::byte data10[1];     // 0x10
};

static_assert(offsetof(TreeNodeOverlay, nodeType) == 0x00);
static_assert(offsetof(TreeNodeOverlay, link04) == 0x04);
static_assert(offsetof(TreeNodeOverlay, next08) == 0x08);
static_assert(offsetof(TreeNodeOverlay, child0C) == 0x0C);
static_assert(offsetof(TreeNodeOverlay, data10) == 0x10);

extern "C" TreeNodeOverlay* __stdcall TreeNode_AllocData(const void* sourceData);

extern "C" TreeNodeOverlay* __stdcall TreeNode_CloneDeep(
    TreeNodeOverlay* source,
    TreeNodeOverlay* link)
{
    TreeNodeOverlay* const rootClone = TreeNode_AllocData(source->data10);
    const std::uint8_t sourceType = source->nodeType;

    rootClone->next08 = nullptr;
    rootClone->child0C = nullptr;
    rootClone->nodeType = sourceType;
    rootClone->link04 = link;

    if (source->child0C != nullptr)
    {
        rootClone->child0C = TreeNode_CloneDeep(source->child0C, rootClone);
    }

    link = rootClone;
    for (TreeNodeOverlay* sibling = source->next08; sibling != nullptr; sibling = sibling->next08)
    {
        TreeNodeOverlay* const siblingClone = TreeNode_AllocData(sibling->data10);
        const std::uint8_t siblingType = sibling->nodeType;

        siblingClone->next08 = nullptr;
        siblingClone->child0C = nullptr;
        siblingClone->nodeType = siblingType;
        link->next08 = siblingClone;
        siblingClone->link04 = link;

        if (sibling->child0C != nullptr)
        {
            siblingClone->child0C = TreeNode_CloneDeep(sibling->child0C, siblingClone);
        }

        link = siblingClone;
    }

    return rootClone;
}
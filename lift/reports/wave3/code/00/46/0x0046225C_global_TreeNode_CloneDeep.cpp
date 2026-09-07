#include <cstddef>
#include <cstdint>

struct TreeNodeOverlay {
    std::uint8_t field_0x0;
    std::uint8_t pad_0x1[3];
    TreeNodeOverlay* field_0x4;
    TreeNodeOverlay* field_0x8;
    TreeNodeOverlay* field_0xC;
    std::uint8_t payload[1];
};

static_assert(offsetof(TreeNodeOverlay, field_0x0) == 0x00);
static_assert(offsetof(TreeNodeOverlay, field_0x4) == 0x04);
static_assert(offsetof(TreeNodeOverlay, field_0x8) == 0x08);
static_assert(offsetof(TreeNodeOverlay, field_0xC) == 0x0C);
static_assert(offsetof(TreeNodeOverlay, payload) == 0x10);

extern "C" void* TreeNode_AllocData(const void* sourceData);
extern "C" TreeNodeOverlay* __stdcall TreeNode_CloneDeep(TreeNodeOverlay* source, TreeNodeOverlay* parent);

extern "C" TreeNodeOverlay* __stdcall TreeNode_CloneDeep(TreeNodeOverlay* source, TreeNodeOverlay* parent)
{
    auto* rootClone = static_cast<TreeNodeOverlay*>(TreeNode_AllocData(&source->payload[0]));
    rootClone->field_0x8 = nullptr;
    rootClone->field_0xC = nullptr;
    rootClone->field_0x0 = source->field_0x0;
    rootClone->field_0x4 = parent;

    if (source->field_0xC != nullptr) {
        rootClone->field_0xC = TreeNode_CloneDeep(source->field_0xC, rootClone);
    }

    TreeNodeOverlay* cloneTail = rootClone;
    for (TreeNodeOverlay* sibling = source->field_0x8; sibling != nullptr; sibling = sibling->field_0x8) {
        auto* siblingClone = static_cast<TreeNodeOverlay*>(TreeNode_AllocData(&sibling->payload[0]));
        siblingClone->field_0x8 = nullptr;
        siblingClone->field_0xC = nullptr;
        siblingClone->field_0x0 = sibling->field_0x0;
        cloneTail->field_0x8 = siblingClone;
        siblingClone->field_0x4 = cloneTail;

        if (sibling->field_0xC != nullptr) {
            siblingClone->field_0xC = TreeNode_CloneDeep(sibling->field_0xC, siblingClone);
        }

        cloneTail = siblingClone;
    }

    return rootClone;
}
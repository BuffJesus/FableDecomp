#include <cstddef>
#include <cstdint>

struct TreeNodeOverlay {
    std::uint8_t field_00;
    std::uint8_t pad_01[3];
    TreeNodeOverlay* field_04;
    TreeNodeOverlay* field_08;
    TreeNodeOverlay* field_0C;
    std::uint8_t payload[1];
};

static_assert(offsetof(TreeNodeOverlay, field_00) == 0x00);
static_assert(offsetof(TreeNodeOverlay, field_04) == 0x04);
static_assert(offsetof(TreeNodeOverlay, field_08) == 0x08);
static_assert(offsetof(TreeNodeOverlay, field_0C) == 0x0C);
static_assert(offsetof(TreeNodeOverlay, payload) == 0x10);

extern "C" void* __stdcall TreeNode_AllocData(const void* sourceData);
extern "C" TreeNodeOverlay* __stdcall TreeNode_CloneDeep(TreeNodeOverlay* source, TreeNodeOverlay* parent);

extern "C" TreeNodeOverlay* __stdcall TreeNode_CloneDeep(TreeNodeOverlay* source, TreeNodeOverlay* parent)
{
    auto* const rootClone = static_cast<TreeNodeOverlay*>(TreeNode_AllocData(&source->payload[0]));
    const std::uint8_t rootValue = source->field_00;

    rootClone->field_08 = nullptr;
    rootClone->field_0C = nullptr;
    rootClone->field_00 = rootValue;
    rootClone->field_04 = parent;

    if (source->field_0C != nullptr) {
        rootClone->field_0C = TreeNode_CloneDeep(source->field_0C, rootClone);
    }

    parent = rootClone;
    for (TreeNodeOverlay* sibling = source->field_08; sibling != nullptr; sibling = sibling->field_08) {
        auto* const siblingClone = static_cast<TreeNodeOverlay*>(TreeNode_AllocData(&sibling->payload[0]));
        const std::uint8_t siblingValue = sibling->field_00;

        siblingClone->field_08 = nullptr;
        siblingClone->field_0C = nullptr;
        siblingClone->field_00 = siblingValue;
        parent->field_08 = siblingClone;
        siblingClone->field_04 = parent;

        if (sibling->field_0C != nullptr) {
            siblingClone->field_0C = TreeNode_CloneDeep(sibling->field_0C, siblingClone);
        }

        parent = siblingClone;
    }

    return rootClone;
}
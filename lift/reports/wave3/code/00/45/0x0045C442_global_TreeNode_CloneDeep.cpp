#include <cstddef>
#include <cstdint>

struct TreeNodeCloneDeepOverlay {
    std::uint8_t valueAt00;              // +0x00
    std::byte pad_01[0x03];
    TreeNodeCloneDeepOverlay* parent;    // +0x04
    TreeNodeCloneDeepOverlay* nextAt08;  // +0x08
    TreeNodeCloneDeepOverlay* nextAt0C;  // +0x0C
    std::byte payload[0x08];             // +0x10
};

static_assert(offsetof(TreeNodeCloneDeepOverlay, parent) == 0x04);
static_assert(offsetof(TreeNodeCloneDeepOverlay, nextAt08) == 0x08);
static_assert(offsetof(TreeNodeCloneDeepOverlay, nextAt0C) == 0x0C);
static_assert(offsetof(TreeNodeCloneDeepOverlay, payload) == 0x10);
static_assert(sizeof(TreeNodeCloneDeepOverlay) == 0x18);

struct TreeNodeCloneDeepContext;

TreeNodeCloneDeepOverlay* __thiscall TreeNode_AllocData(
    TreeNodeCloneDeepContext* self,
    void* sourceData);

TreeNodeCloneDeepOverlay* __thiscall TreeNode_CloneDeep(
    TreeNodeCloneDeepContext* self,
    TreeNodeCloneDeepOverlay* sourceNode,
    TreeNodeCloneDeepOverlay* parentNode)
{
    auto* const clonedRoot =
        TreeNode_AllocData(self, static_cast<void*>(&sourceNode->payload[0]));
    const std::uint8_t rootValue = sourceNode->valueAt00;

    clonedRoot->nextAt08 = nullptr;
    clonedRoot->nextAt0C = nullptr;
    clonedRoot->valueAt00 = rootValue;
    clonedRoot->parent = parentNode;

    if (sourceNode->nextAt0C != nullptr) {
        clonedRoot->nextAt0C = TreeNode_CloneDeep(self, sourceNode->nextAt0C, clonedRoot);
    }

    parentNode = clonedRoot;
    for (auto* sibling = sourceNode->nextAt08; sibling != nullptr; sibling = sibling->nextAt08) {
        auto* const clonedSibling =
            TreeNode_AllocData(self, static_cast<void*>(&sibling->payload[0]));
        const std::uint8_t siblingValue = sibling->valueAt00;

        clonedSibling->nextAt08 = nullptr;
        clonedSibling->nextAt0C = nullptr;
        clonedSibling->valueAt00 = siblingValue;
        parentNode->nextAt08 = clonedSibling;
        clonedSibling->parent = parentNode;

        if (sibling->nextAt0C != nullptr) {
            clonedSibling->nextAt0C = TreeNode_CloneDeep(self, sibling->nextAt0C, clonedSibling);
        }

        parentNode = clonedSibling;
    }

    return clonedRoot;
}
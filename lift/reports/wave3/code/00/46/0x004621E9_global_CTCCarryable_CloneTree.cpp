#include <cstddef>

class CTCCarryable {
public:
    void __thiscall OnKill();
};

struct CTCCarryable_TreeOverlay {
    std::byte pad_00[0x04];
    void* root;         // 0x04
    void* terminalAt08; // 0x08
    void* terminalAt0C; // 0x0C
};
static_assert(offsetof(CTCCarryable_TreeOverlay, root) == 0x04);
static_assert(offsetof(CTCCarryable_TreeOverlay, terminalAt08) == 0x08);
static_assert(offsetof(CTCCarryable_TreeOverlay, terminalAt0C) == 0x0C);

struct CTCCarryable_Overlay {
    CTCCarryable_TreeOverlay* tree; // 0x00
    void* linkAt04;                 // 0x04
};
static_assert(offsetof(CTCCarryable_Overlay, tree) == 0x00);
static_assert(offsetof(CTCCarryable_Overlay, linkAt04) == 0x04);

struct TreeNodeOverlay {
    std::byte pad_00[0x08];
    TreeNodeOverlay* nextAt08; // 0x08
    TreeNodeOverlay* nextAt0C; // 0x0C
};
static_assert(offsetof(TreeNodeOverlay, nextAt08) == 0x08);
static_assert(offsetof(TreeNodeOverlay, nextAt0C) == 0x0C);

// Observed call shape at 0x0046225C:
//   ECX = self
//   push selfOverlay->tree
//   push otherOverlay->tree->root
//   call TreeNode_CloneDeep
extern TreeNodeOverlay* __thiscall TreeNode_CloneDeep(
    CTCCarryable* self,
    void* sourceRoot,
    void* treeOwner);

CTCCarryable* __thiscall CTCCarryable_CloneTree(CTCCarryable* self, CTCCarryable* other)
{
    if (self != other) {
        self->CTCCarryable::OnKill();

        auto* const selfOverlay = reinterpret_cast<CTCCarryable_Overlay*>(self);
        auto* const otherOverlay = reinterpret_cast<CTCCarryable_Overlay*>(other);

        selfOverlay->linkAt04 = nullptr;

        if (otherOverlay->tree->root == nullptr) {
            selfOverlay->tree->root = nullptr;
            selfOverlay->tree->terminalAt08 = selfOverlay->tree;
            selfOverlay->tree->terminalAt0C = selfOverlay->tree;
        } else {
            CTCCarryable_TreeOverlay* const selfTree = selfOverlay->tree;
            selfTree->root = TreeNode_CloneDeep(self, otherOverlay->tree->root, selfTree);

            auto* nodeAt08 = static_cast<TreeNodeOverlay*>(selfTree->root);
            TreeNodeOverlay* lastAt08;
            do {
                lastAt08 = nodeAt08;
                nodeAt08 = lastAt08->nextAt08;
            } while (nodeAt08 != nullptr);
            selfTree->terminalAt08 = lastAt08;

            auto* nodeAt0C = static_cast<TreeNodeOverlay*>(selfTree->root);
            TreeNodeOverlay* lastAt0C;
            do {
                lastAt0C = nodeAt0C;
                nodeAt0C = lastAt0C->nextAt0C;
            } while (nodeAt0C != nullptr);
            selfTree->terminalAt0C = lastAt0C;

            selfOverlay->linkAt04 = otherOverlay->linkAt04;
        }
    }

    return self;
}
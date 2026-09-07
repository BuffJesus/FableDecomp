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

// Observed call shape:
//   ECX = self
//   push selfOverlay->tree
//   push otherOverlay->tree->root
//   call TreeNode_CloneDeep
TreeNodeOverlay* __thiscall TreeNode_CloneDeep(
    CTCCarryable* self,
    void* sourceRoot,
    void* treeOwner);

CTCCarryable* __thiscall CTCCarryable_AssignClonedTree(CTCCarryable* self, CTCCarryable* other)
{
    auto* const selfOverlay = reinterpret_cast<CTCCarryable_Overlay*>(self);
    auto* const otherOverlay = reinterpret_cast<CTCCarryable_Overlay*>(other);

    if (self != other) {
        self->CTCCarryable::OnKill();
        selfOverlay->linkAt04 = nullptr;

        if (otherOverlay->tree->root == nullptr) {
            selfOverlay->tree->root = nullptr;
            selfOverlay->tree->terminalAt08 = selfOverlay->tree;
            selfOverlay->tree->terminalAt0C = selfOverlay->tree;
        } else {
            CTCCarryable_TreeOverlay* const selfTree = selfOverlay->tree;
            selfTree->root = TreeNode_CloneDeep(self, otherOverlay->tree->root, selfTree);

            auto* node08 = static_cast<TreeNodeOverlay*>(selfTree->root);
            TreeNodeOverlay* last08;
            do {
                last08 = node08;
                node08 = last08->nextAt08;
            } while (node08 != nullptr);
            selfTree->terminalAt08 = last08;

            auto* node0C = static_cast<TreeNodeOverlay*>(selfTree->root);
            TreeNodeOverlay* last0C;
            do {
                last0C = node0C;
                node0C = last0C->nextAt0C;
            } while (node0C != nullptr);
            selfTree->terminalAt0C = last0C;

            selfOverlay->linkAt04 = otherOverlay->linkAt04;
        }
    }

    return self;
}
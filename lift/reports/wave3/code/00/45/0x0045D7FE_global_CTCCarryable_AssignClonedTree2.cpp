#include <cstddef>

struct TreeNodeOverlay {
    std::byte pad0[8];
    TreeNodeOverlay* at_0x08;
    TreeNodeOverlay* at_0x0C;
};

static_assert(offsetof(TreeNodeOverlay, at_0x08) == 0x08);
static_assert(offsetof(TreeNodeOverlay, at_0x0C) == 0x0C);

struct TreeHeaderOverlay {
    std::byte pad0[4];
    TreeNodeOverlay* root;
    TreeNodeOverlay* at_0x08;
    TreeNodeOverlay* at_0x0C;
};

static_assert(offsetof(TreeHeaderOverlay, root) == 0x04);
static_assert(offsetof(TreeHeaderOverlay, at_0x08) == 0x08);
static_assert(offsetof(TreeHeaderOverlay, at_0x0C) == 0x0C);

struct CTCCarryableOverlay {
    TreeHeaderOverlay* treeHeader;
    void* at_0x04;
};

static_assert(offsetof(CTCCarryableOverlay, treeHeader) == 0x00);
static_assert(offsetof(CTCCarryableOverlay, at_0x04) == 0x04);

extern TreeNodeOverlay* __cdecl TreeNode_CloneDeep(TreeNodeOverlay* node, TreeHeaderOverlay* header);

CTCCarryable* __thiscall CTCCarryable_AssignClonedTree2(CTCCarryable* self, CTCCarryable* other) {
    if (self != other) {
        auto* selfView = reinterpret_cast<CTCCarryableOverlay*>(self);
        auto* otherView = reinterpret_cast<CTCCarryableOverlay*>(other);

        CTCCarryable::OnKill(self);
        selfView->at_0x04 = nullptr;

        if (otherView->treeHeader->root == nullptr) {
            selfView->treeHeader->root = nullptr;
            selfView->treeHeader->at_0x08 = reinterpret_cast<TreeNodeOverlay*>(selfView->treeHeader);
            selfView->treeHeader->at_0x0C = reinterpret_cast<TreeNodeOverlay*>(selfView->treeHeader);
        } else {
            TreeHeaderOverlay* const selfHeader = selfView->treeHeader;
            selfHeader->root = TreeNode_CloneDeep(otherView->treeHeader->root, selfHeader);

            TreeNodeOverlay* node = selfView->treeHeader->root;
            TreeNodeOverlay* last;
            do {
                last = node;
                node = last->at_0x08;
            } while (node != nullptr);
            selfView->treeHeader->at_0x08 = last;

            node = selfView->treeHeader->root;
            do {
                last = node;
                node = last->at_0x0C;
            } while (node != nullptr);
            selfView->treeHeader->at_0x0C = last;

            selfView->at_0x04 = otherView->at_0x04;
        }
    }

    return self;
}
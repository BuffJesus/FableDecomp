#include <cstddef>
#include <cstdint>

struct TCCarryableTreeNode;

struct TCCarryableTreeHeader {
    std::byte pad00[0x04];
    TCCarryableTreeNode* root;       // 0x04
    TCCarryableTreeNode* leftmost;   // 0x08
    TCCarryableTreeNode* rightmost;  // 0x0C
};

struct TCCarryableTreeNode {
    std::byte pad00[0x08];
    TCCarryableTreeNode* left;       // 0x08
    TCCarryableTreeNode* right;      // 0x0C
};

struct CTCCarryable_Overlay {
    TCCarryableTreeHeader* treeHeader; // 0x00
    std::int32_t copyTag;              // 0x04
};

static_assert(offsetof(TCCarryableTreeHeader, root) == 0x04);
static_assert(offsetof(TCCarryableTreeHeader, leftmost) == 0x08);
static_assert(offsetof(TCCarryableTreeHeader, rightmost) == 0x0C);
static_assert(offsetof(TCCarryableTreeNode, left) == 0x08);
static_assert(offsetof(TCCarryableTreeNode, right) == 0x0C);
static_assert(offsetof(CTCCarryable_Overlay, treeHeader) == 0x00);
static_assert(offsetof(CTCCarryable_Overlay, copyTag) == 0x04);

class CTCCarryable;

extern TCCarryableTreeNode* __cdecl Tree_Clone_Recursive(TCCarryableTreeNode* sourceRoot, TCCarryableTreeHeader* ownerHeader);

CTCCarryable* __thiscall CTCCarryable_CopyConstructor(CTCCarryable* self, CTCCarryable* other)
{
    auto* const selfView = reinterpret_cast<CTCCarryable_Overlay*>(self);
    auto* const otherView = reinterpret_cast<CTCCarryable_Overlay*>(other);

    if (self != other) {
        self->OnKill();
        selfView->copyTag = 0;

        if (otherView->treeHeader->root == nullptr) {
            selfView->treeHeader->root = nullptr;
            selfView->treeHeader->leftmost = reinterpret_cast<TCCarryableTreeNode*>(selfView->treeHeader);
            selfView->treeHeader->rightmost = reinterpret_cast<TCCarryableTreeNode*>(selfView->treeHeader);
        } else {
            TCCarryableTreeHeader* const ownerHeader = selfView->treeHeader;
            ownerHeader->root = Tree_Clone_Recursive(otherView->treeHeader->root, ownerHeader);

            TCCarryableTreeNode* node = selfView->treeHeader->root;
            TCCarryableTreeNode* last;
            do {
                last = node;
                node = last->left;
            } while (node != nullptr);
            selfView->treeHeader->leftmost = last;

            node = selfView->treeHeader->root;
            do {
                last = node;
                node = last->right;
            } while (node != nullptr);
            selfView->treeHeader->rightmost = last;

            selfView->copyTag = otherView->copyTag;
        }
    }

    return self;
}
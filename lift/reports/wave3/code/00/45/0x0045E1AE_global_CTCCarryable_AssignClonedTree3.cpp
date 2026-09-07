#include <cstddef>
#include <cstdint>

struct TreeNode;
class CTCCarryable;

extern TreeNode* __cdecl TreeNode_CloneDeep(TreeNode* node, void* owner);

class CTCCarryable {
public:
    static void __thiscall OnKill(CTCCarryable* self);
};

struct TreeNode {
    std::byte pad_00[0x08];
    TreeNode* child_at_08;
    TreeNode* child_at_0C;
};

static_assert(offsetof(TreeNode, child_at_08) == 0x08);
static_assert(offsetof(TreeNode, child_at_0C) == 0x0C);

struct CarryTreeOverlay {
    std::byte pad_00[0x04];
    TreeNode* root;
    TreeNode* tail_at_08;
    TreeNode* tail_at_0C;
};

static_assert(offsetof(CarryTreeOverlay, root) == 0x04);
static_assert(offsetof(CarryTreeOverlay, tail_at_08) == 0x08);
static_assert(offsetof(CarryTreeOverlay, tail_at_0C) == 0x0C);

struct CTCCarryableOverlay {
    CarryTreeOverlay* tree;
    void* field_04;
};

static_assert(offsetof(CTCCarryableOverlay, tree) == 0x00);
static_assert(offsetof(CTCCarryableOverlay, field_04) == 0x04);

CTCCarryable* __thiscall CTCCarryable_AssignClonedTree3(
    CTCCarryable* self,
    CTCCarryable* other)
{
    auto* selfView = reinterpret_cast<CTCCarryableOverlay*>(self);
    auto* otherView = reinterpret_cast<CTCCarryableOverlay*>(other);

    if (self != other) {
        CTCCarryable::OnKill(self);
        selfView->field_04 = nullptr;

        if (otherView->tree->root == nullptr) {
            selfView->tree->root = nullptr;
            selfView->tree->tail_at_08 = reinterpret_cast<TreeNode*>(selfView->tree);
            selfView->tree->tail_at_0C = reinterpret_cast<TreeNode*>(selfView->tree);
        } else {
            CarryTreeOverlay* const selfTree = selfView->tree;
            selfTree->root = TreeNode_CloneDeep(otherView->tree->root, selfTree);

            TreeNode* node = selfTree->root;
            TreeNode* last;
            do {
                last = node;
                node = last->child_at_08;
            } while (node != nullptr);
            selfTree->tail_at_08 = last;

            node = selfTree->root;
            do {
                last = node;
                node = last->child_at_0C;
            } while (node != nullptr);
            selfTree->tail_at_0C = last;

            selfView->field_04 = otherView->field_04;
        }
    }

    return self;
}
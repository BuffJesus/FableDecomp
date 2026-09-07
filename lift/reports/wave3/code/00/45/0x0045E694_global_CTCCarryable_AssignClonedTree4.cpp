#include <cstddef>
#include <cstdint>

struct CTCCarryable;
struct TreeNode;

struct CTCCarryableTreeOverlay {
    void* unk0;
    TreeNode* root;
    TreeNode* leftmost;
    TreeNode* rightmost;
};

static_assert(offsetof(CTCCarryableTreeOverlay, root) == 0x4);
static_assert(offsetof(CTCCarryableTreeOverlay, leftmost) == 0x8);
static_assert(offsetof(CTCCarryableTreeOverlay, rightmost) == 0xC);

struct TreeNode {
    std::byte pad_00[0x8];
    TreeNode* child_at_08;
    TreeNode* child_at_0C;
};

static_assert(offsetof(TreeNode, child_at_08) == 0x8);
static_assert(offsetof(TreeNode, child_at_0C) == 0xC);

struct CTCCarryableAssignOverlay {
    CTCCarryableTreeOverlay* tree;
    std::uint32_t field_04;
};

static_assert(offsetof(CTCCarryableAssignOverlay, tree) == 0x0);
static_assert(offsetof(CTCCarryableAssignOverlay, field_04) == 0x4);

struct CTCCarryable {
    void OnKill();
};

extern TreeNode* TreeNode_CloneDeep(TreeNode* root, CTCCarryableTreeOverlay* owner_tree);

CTCCarryable* __thiscall CTCCarryable_AssignClonedTree4(CTCCarryable* self, CTCCarryable* other) {
    auto* self_overlay = reinterpret_cast<CTCCarryableAssignOverlay*>(self);
    auto* other_overlay = reinterpret_cast<CTCCarryableAssignOverlay*>(other);

    if (self != other) {
        self->OnKill();
        self_overlay->field_04 = 0;

        if (other_overlay->tree->root == nullptr) {
            self_overlay->tree->root = nullptr;
            self_overlay->tree->leftmost = reinterpret_cast<TreeNode*>(self_overlay->tree);
            self_overlay->tree->rightmost = reinterpret_cast<TreeNode*>(self_overlay->tree);
        } else {
            CTCCarryableTreeOverlay* const self_tree = self_overlay->tree;
            self_tree->root = TreeNode_CloneDeep(other_overlay->tree->root, self_tree);

            TreeNode* node = self_overlay->tree->root;
            TreeNode* last;
            do {
                last = node;
                node = last->child_at_08;
            } while (node != nullptr);
            self_overlay->tree->leftmost = last;

            node = self_overlay->tree->root;
            do {
                last = node;
                node = last->child_at_0C;
            } while (node != nullptr);
            self_overlay->tree->rightmost = last;

            self_overlay->field_04 = other_overlay->field_04;
        }
    }

    return self;
}
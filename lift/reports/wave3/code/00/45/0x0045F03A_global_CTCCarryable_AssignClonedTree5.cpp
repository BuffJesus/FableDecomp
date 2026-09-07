#include <cstddef>
#include <cstdint>

struct TreeNodeOverlay {
    std::uint8_t pad_0x00[0x4];
    TreeNodeOverlay* root;   // 0x04
    TreeNodeOverlay* left;   // 0x08
    TreeNodeOverlay* right;  // 0x0C
};

static_assert(offsetof(TreeNodeOverlay, root) == 0x04);
static_assert(offsetof(TreeNodeOverlay, left) == 0x08);
static_assert(offsetof(TreeNodeOverlay, right) == 0x0C);

struct CTCCarryableOverlay {
    TreeNodeOverlay* tree;       // 0x00
    std::uint32_t field_0x04;    // 0x04
};

static_assert(offsetof(CTCCarryableOverlay, tree) == 0x00);
static_assert(offsetof(CTCCarryableOverlay, field_0x04) == 0x04);

class CTCCarryable {
public:
    void OnKill();
};

TreeNodeOverlay* TreeNode_CloneDeep(TreeNodeOverlay* node, TreeNodeOverlay* owner);

CTCCarryable* __thiscall CTCCarryable_AssignClonedTree5(CTCCarryable* self, CTCCarryable* other)
{
    auto* self_view = reinterpret_cast<CTCCarryableOverlay*>(self);
    auto* other_view = reinterpret_cast<CTCCarryableOverlay*>(other);

    if (self != other) {
        self->OnKill();
        self_view->field_0x04 = 0;

        if (other_view->tree->root == nullptr) {
            self_view->tree->root = nullptr;
            self_view->tree->left = self_view->tree;
            self_view->tree->right = self_view->tree;
        } else {
            TreeNodeOverlay* const tree = self_view->tree;
            tree->root = TreeNode_CloneDeep(other_view->tree->root, tree);

            TreeNodeOverlay* node = self_view->tree->root;
            TreeNodeOverlay* last;
            do {
                last = node;
                node = last->left;
            } while (node != nullptr);
            self_view->tree->left = last;

            node = self_view->tree->root;
            do {
                last = node;
                node = last->right;
            } while (node != nullptr);
            self_view->tree->right = last;

            self_view->field_0x04 = other_view->field_0x04;
        }
    }

    return self;
}
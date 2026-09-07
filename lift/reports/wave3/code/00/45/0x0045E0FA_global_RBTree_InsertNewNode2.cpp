#include <cstddef>
#include <cstdint>

struct RBTreeNodeOverlay {
    std::uint32_t reserved0;
    RBTreeNodeOverlay* parent;
    RBTreeNodeOverlay* left;
    RBTreeNodeOverlay* right;
    std::int32_t value;
};

static_assert(offsetof(RBTreeNodeOverlay, parent) == 0x04);
static_assert(offsetof(RBTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, value) == 0x10);

struct RBTreeOverlay {
    RBTreeNodeOverlay* head;
    std::int32_t count;
};

static_assert(offsetof(RBTreeOverlay, head) == 0x00);
static_assert(offsetof(RBTreeOverlay, count) == 0x04);

extern "C" RBTreeNodeOverlay* __thiscall TreeNode_AllocData_Byte(const std::int32_t* value);
extern "C" void __cdecl CMemoryAllocatorVariableSize__BalanceTree(
    RBTreeNodeOverlay* node,
    RBTreeNodeOverlay** root_link
);

void __thiscall RBTree_InsertNewNode2(
    RBTreeOverlay* self,
    RBTreeNodeOverlay** out_node,
    int param_3,
    RBTreeNodeOverlay* parent,
    const std::int32_t* value,
    int param_6
) {
    RBTreeNodeOverlay* new_node;
    RBTreeNodeOverlay* head;

    if ((parent == self->head) ||
        ((param_6 == 0) && ((param_3 != 0 || (*value < parent->value))))) {
        new_node = TreeNode_AllocData_Byte(value);
        parent->left = new_node;

        head = self->head;
        if (parent != head) {
            if (parent == head->left) {
                head->left = new_node;
            }
            goto link_done;
        }

        head->parent = new_node;
        head = self->head;
    } else {
        new_node = TreeNode_AllocData_Byte(value);
        parent->right = new_node;

        head = self->head;
        if (parent != head->right) {
            goto link_done;
        }
    }

    head->right = new_node;

link_done:
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->parent = parent;
    CMemoryAllocatorVariableSize__BalanceTree(new_node, &self->head->parent);
    self->count = self->count + 1;
    *out_node = new_node;
}
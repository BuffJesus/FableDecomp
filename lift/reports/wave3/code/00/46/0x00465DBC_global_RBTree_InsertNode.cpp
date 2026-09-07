#include <cstddef>
#include <cstdint>

struct RBTreeNodeOverlay {
    std::uint8_t pad_0x00[0x04];
    RBTreeNodeOverlay* parent;   // 0x04
    RBTreeNodeOverlay* left;     // 0x08
    RBTreeNodeOverlay* right;    // 0x0C
    int value_at_0x10;           // 0x10
};

static_assert(offsetof(RBTreeNodeOverlay, parent) == 0x04);
static_assert(offsetof(RBTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, value_at_0x10) == 0x10);

struct RBTreeOverlay {
    RBTreeNodeOverlay* head; // 0x00
    int count;               // 0x04
};

static_assert(offsetof(RBTreeOverlay, head) == 0x00);
static_assert(offsetof(RBTreeOverlay, count) == 0x04);

extern "C" RBTreeNodeOverlay* __thiscall Allocator_Alloc_Struct_Smaller(
    RBTreeOverlay* self,
    int* param_5
);

extern "C" void __cdecl CMemoryAllocatorVariableSize__BalanceTree(
    RBTreeNodeOverlay* node,
    RBTreeNodeOverlay** root_link
);

void __thiscall RBTree_InsertNode(
    RBTreeOverlay* self,
    RBTreeNodeOverlay** inserted_node_out,
    int param_3,
    RBTreeNodeOverlay* param_4,
    int* param_5,
    int param_6
) {
    RBTreeNodeOverlay* new_node;
    RBTreeNodeOverlay* head_node;

    if ((param_4 == self->head) ||
        ((param_6 == 0) && ((param_3 != 0) || (*param_5 < param_4->value_at_0x10)))) {
        new_node = Allocator_Alloc_Struct_Smaller(self, param_5);
        param_4->left = new_node;
        head_node = self->head;

        if (param_4 != head_node) {
            if (param_4 == head_node->left) {
                head_node->left = new_node;
            }
            goto link_common;
        }

        head_node->parent = new_node;
        head_node = self->head;
    } else {
        new_node = Allocator_Alloc_Struct_Smaller(self, param_5);
        param_4->right = new_node;
        head_node = self->head;

        if (param_4 != head_node->right) {
            goto link_common;
        }
    }

    head_node->right = new_node;

link_common:
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->parent = param_4;
    CMemoryAllocatorVariableSize__BalanceTree(new_node, &self->head->parent);
    self->count = self->count + 1;
    *inserted_node_out = new_node;
}
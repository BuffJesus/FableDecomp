#include <cstddef>
#include <cstdint>

struct UIntSetNodeOverlay {
    std::uint32_t unknown_0x00;
    UIntSetNodeOverlay* parent;
    UIntSetNodeOverlay* left;
    UIntSetNodeOverlay* right;
    std::uint32_t value;
};

static_assert(offsetof(UIntSetNodeOverlay, parent) == 0x04);
static_assert(offsetof(UIntSetNodeOverlay, left) == 0x08);
static_assert(offsetof(UIntSetNodeOverlay, right) == 0x0C);
static_assert(offsetof(UIntSetNodeOverlay, value) == 0x10);

struct UIntSetTreeOverlay {
    UIntSetNodeOverlay* header;
    std::int32_t size;
};

static_assert(offsetof(UIntSetTreeOverlay, header) == 0x00);
static_assert(offsetof(UIntSetTreeOverlay, size) == 0x04);

extern "C" UIntSetNodeOverlay* __thiscall TreeNode_AllocData(
    UIntSetTreeOverlay* tree,
    const std::uint32_t* value
);

extern "C" void __cdecl CMemoryAllocatorVariableSize__BalanceTree(
    UIntSetNodeOverlay* node,
    void* root_link
);

void __thiscall ::std_set_insert_uint_via_FUN(
    UIntSetTreeOverlay* tree,
    UIntSetNodeOverlay** inserted_node,
    int param_3,
    UIntSetNodeOverlay* parent,
    const std::uint32_t* value,
    int param_6
) {
    UIntSetNodeOverlay* new_node;
    UIntSetNodeOverlay* header;

    if ((parent == tree->header) ||
        ((param_6 == 0) && ((param_3 != 0) || (*value < parent->value)))) {
        new_node = TreeNode_AllocData(tree, value);
        parent->left = new_node;

        header = tree->header;
        if (parent != header) {
            if (parent == header->left) {
                header->left = new_node;
            }
            goto link_complete;
        }

        header->parent = new_node;
        header = tree->header;
    } else {
        new_node = TreeNode_AllocData(tree, value);
        parent->right = new_node;

        header = tree->header;
        if (parent != header->right) {
            goto link_complete;
        }
    }

    header->right = new_node;

link_complete:
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->parent = parent;
    CMemoryAllocatorVariableSize__BalanceTree(
        new_node,
        reinterpret_cast<std::uint8_t*>(tree->header) + 0x04
    );
    ++tree->size;
    *inserted_node = new_node;
}
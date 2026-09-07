#include <cstddef>
#include <cstdint>

struct RBTreeNodeOverlay {
    std::uint32_t unk0;
    RBTreeNodeOverlay* parent;
    RBTreeNodeOverlay* left;
    RBTreeNodeOverlay* right;
    std::int32_t key;
};

static_assert(offsetof(RBTreeNodeOverlay, parent) == 0x04);
static_assert(offsetof(RBTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, key) == 0x10);

struct RBTreeOverlay {
    RBTreeNodeOverlay* header;
    std::int32_t size;
};

static_assert(offsetof(RBTreeOverlay, header) == 0x00);
static_assert(offsetof(RBTreeOverlay, size) == 0x04);

extern "C" RBTreeNodeOverlay* __cdecl TreeNode_AllocData(const std::int32_t* value);
extern "C" void __cdecl CMemoryAllocatorVariableSize__BalanceTree(
    RBTreeNodeOverlay* node,
    void* root_slot
);

void __thiscall RBTree_Insert_Internal(
    RBTreeOverlay* self,
    RBTreeNodeOverlay** outInserted,
    int param_3,
    RBTreeNodeOverlay* parent,
    const std::int32_t* value,
    int param_6
) {
    RBTreeNodeOverlay* newNode;
    RBTreeNodeOverlay* header;

    if ((parent == self->header) ||
        ((param_6 == 0) && ((param_3 != 0) || (*value < parent->key)))) {
        newNode = TreeNode_AllocData(value);
        parent->left = newNode;
        header = self->header;
        if (parent != header) {
            if (parent == header->left) {
                header->left = newNode;
            }
            goto finish_link;
        }
        header->parent = newNode;
        header = self->header;
    } else {
        newNode = TreeNode_AllocData(value);
        parent->right = newNode;
        header = self->header;
        if (parent != header->right) {
            goto finish_link;
        }
    }

    header->right = newNode;

finish_link:
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->parent = parent;
    CMemoryAllocatorVariableSize__BalanceTree(
        newNode,
        reinterpret_cast<std::uint8_t*>(self->header) + 0x04
    );
    self->size = self->size + 1;
    *outInserted = newNode;
}
#include <cstddef>
#include <cstdint>

struct CMemoryAllocatorVariableSize_Overlay;
struct CMemoryAllocatorVariableSize_Node_Overlay;

struct FindOrInsertResult_Overlay {
    CMemoryAllocatorVariableSize_Node_Overlay* node;
    bool inserted;
    std::uint8_t padding[3];
};
static_assert(offsetof(FindOrInsertResult_Overlay, node) == 0x00);
static_assert(offsetof(FindOrInsertResult_Overlay, inserted) == 0x04);
static_assert(sizeof(FindOrInsertResult_Overlay) == 0x08);

struct CMemoryAllocatorVariableSize_Node_Overlay {
    std::uint8_t pad_00[0x08];
    CMemoryAllocatorVariableSize_Node_Overlay* left;   // 0x08
    CMemoryAllocatorVariableSize_Node_Overlay* right;  // 0x0C
    std::uint32_t key;                                 // 0x10
};
static_assert(offsetof(CMemoryAllocatorVariableSize_Node_Overlay, left) == 0x08);
static_assert(offsetof(CMemoryAllocatorVariableSize_Node_Overlay, right) == 0x0C);
static_assert(offsetof(CMemoryAllocatorVariableSize_Node_Overlay, key) == 0x10);

struct CMemoryAllocatorVariableSize_Tree_Overlay {
    std::uint8_t pad_00[0x04];
    CMemoryAllocatorVariableSize_Node_Overlay* root;      // 0x04
    CMemoryAllocatorVariableSize_Node_Overlay* sentinel;  // 0x08
};
static_assert(offsetof(CMemoryAllocatorVariableSize_Tree_Overlay, root) == 0x04);
static_assert(offsetof(CMemoryAllocatorVariableSize_Tree_Overlay, sentinel) == 0x08);

struct CMemoryAllocatorVariableSize_Overlay {
    CMemoryAllocatorVariableSize_Tree_Overlay* tree; // 0x00
};
static_assert(offsetof(CMemoryAllocatorVariableSize_Overlay, tree) == 0x00);

extern CMemoryAllocatorVariableSize_Node_Overlay* __cdecl RBTree__GetMaxNode(
    CMemoryAllocatorVariableSize_Node_Overlay* node);

extern FindOrInsertResult_Overlay* __thiscall BinaryTree_InsertScriptData(
    CMemoryAllocatorVariableSize_Overlay* self,
    std::uint32_t** key_local,
    CMemoryAllocatorVariableSize_Node_Overlay* insert_at,
    CMemoryAllocatorVariableSize_Node_Overlay* parent,
    std::uint32_t* key,
    int zero_flag);

FindOrInsertResult_Overlay* __thiscall CMemoryAllocatorVariableSize_FindOrInsert(
    CMemoryAllocatorVariableSize_Overlay* self,
    FindOrInsertResult_Overlay* out_result,
    std::uint32_t* key_ptr)
{
    CMemoryAllocatorVariableSize_Tree_Overlay* const tree = self->tree;
    CMemoryAllocatorVariableSize_Node_Overlay* current = tree->root;
    bool went_left = true;
    CMemoryAllocatorVariableSize_Node_Overlay* parent = reinterpret_cast<CMemoryAllocatorVariableSize_Node_Overlay*>(tree);

    if (current != nullptr) {
        do {
            parent = current;
            went_left = (*key_ptr < parent->key);
            if (went_left) {
                current = parent->left;
            } else {
                current = parent->right;
            }
        } while (current != nullptr);
    }

    CMemoryAllocatorVariableSize_Node_Overlay* candidate = parent;
    if (went_left) {
        current = parent;
        if (parent == tree->sentinel) {
            goto insert_new_node;
        }
        candidate = RBTree__GetMaxNode(parent);
    }

    current = nullptr;
    if (*key_ptr <= candidate->key) {
        out_result->node = candidate;
        out_result->inserted = false;
        return out_result;
    }

insert_new_node:
    {
        FindOrInsertResult_Overlay* const insert_result =
            BinaryTree_InsertScriptData(self, &key_ptr, current, parent, key_ptr, 0);
        out_result->node = insert_result->node;
        out_result->inserted = true;
        return out_result;
    }
}
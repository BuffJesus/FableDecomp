#include <cstddef>
#include <cstdint>
#include <functional>

struct CCharString;
struct CScriptDataBase;

namespace NScript {
struct CDanyalTestScriptsData : CScriptDataBase {
    static CDanyalTestScriptsData* Alloc();
};
}

void CMemoryAllocatorVariableSize__BalanceTree(CScriptDataBase* node, void* tree_links);

struct CScriptDataBase {
    std::uint8_t pad_0[0x4];
    CScriptDataBase* parent; // 0x04
    CScriptDataBase* left;   // 0x08
    CScriptDataBase* right;  // 0x0C
};
static_assert(offsetof(CScriptDataBase, parent) == 0x04);
static_assert(offsetof(CScriptDataBase, left) == 0x08);
static_assert(offsetof(CScriptDataBase, right) == 0x0C);

struct BinaryTreeCStringNodeOverlay : CScriptDataBase {
    CCharString key; // 0x10
};
static_assert(offsetof(BinaryTreeCStringNodeOverlay, key) == 0x10);

struct BinaryTreeCStringOverlay {
    CScriptDataBase* root_or_head;      // 0x00
    int count;                          // 0x04
    std::less<CCharString> less_cmp;    // 0x08
};
static_assert(offsetof(BinaryTreeCStringOverlay, root_or_head) == 0x00);
static_assert(offsetof(BinaryTreeCStringOverlay, count) == 0x04);
static_assert(offsetof(BinaryTreeCStringOverlay, less_cmp) == 0x08);

void __thiscall BinaryTree_InsertCString(
    BinaryTreeCStringOverlay* self,
    CScriptDataBase** out_node,
    int force_left,
    CScriptDataBase* parent,
    CCharString* key,
    int force_right)
{
    CScriptDataBase* new_node;

    if (parent == self->root_or_head) {
insert_left:
        new_node = NScript::CDanyalTestScriptsData::Alloc();
        parent->left = new_node;

        CScriptDataBase* root_or_head = self->root_or_head;
        if (parent != root_or_head) {
            if (parent == root_or_head->left) {
                root_or_head->left = new_node;
            }
            goto finish_link;
        }

        root_or_head->parent = new_node;
        root_or_head = self->root_or_head;
        root_or_head->right = new_node;
    } else {
        if (force_right == 0) {
            if (force_left == 0) {
                const bool less_result =
                    self->less_cmp(*key, static_cast<BinaryTreeCStringNodeOverlay*>(parent)->key);
                if (!less_result) {
                    goto insert_right;
                }
            }
            goto insert_left;
        }

insert_right:
        new_node = NScript::CDanyalTestScriptsData::Alloc();
        parent->right = new_node;

        CScriptDataBase* root_or_head = self->root_or_head;
        if (parent != root_or_head->right) {
            goto finish_link;
        }

        root_or_head->right = new_node;
    }

finish_link:
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->parent = parent;
    CMemoryAllocatorVariableSize__BalanceTree(new_node, reinterpret_cast<std::uint8_t*>(self->root_or_head) + 0x4);
    self->count = self->count + 1;
    *out_node = new_node;
}
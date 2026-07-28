#include <cstddef>
#include <cstdint>

class CScriptBase;
class CGameScriptInterfaceBase;

template <typename T>
struct CBasicString;

template <>
struct CBasicString<char> {
    static long __cdecl Compare(const char* lhs, const char* rhs);
};

struct CCharStringData {
    char* m_psz;             // 0x00
    std::int32_t m_nLength;  // 0x04
};
static_assert(offsetof(CCharStringData, m_psz) == 0x00);
static_assert(offsetof(CCharStringData, m_nLength) == 0x04);

struct BinaryTreeDigScriptOverlay {
    CScriptBase* root_or_head; // 0x00
    std::int32_t count;        // 0x04
};
static_assert(offsetof(BinaryTreeDigScriptOverlay, root_or_head) == 0x00);
static_assert(offsetof(BinaryTreeDigScriptOverlay, count) == 0x04);

struct BinaryTreeDigScriptNodeOverlay {
    std::byte pad_00[0x04];
    CScriptBase* parent;       // 0x04
    CScriptBase* left;         // 0x08
    CScriptBase* right;        // 0x0C
    CCharStringData* key;      // 0x10
};
static_assert(offsetof(BinaryTreeDigScriptNodeOverlay, parent) == 0x04);
static_assert(offsetof(BinaryTreeDigScriptNodeOverlay, left) == 0x08);
static_assert(offsetof(BinaryTreeDigScriptNodeOverlay, right) == 0x0C);
static_assert(offsetof(BinaryTreeDigScriptNodeOverlay, key) == 0x10);

namespace NScript {
class CExpression_DigScript : public CScriptBase {
public:
    static CExpression_DigScript* __fastcall Alloc(
        BinaryTreeDigScriptOverlay* self,
        CGameScriptInterfaceBase* scriptInterface);
};
}

extern "C" void __cdecl CMemoryAllocatorVariableSize__BalanceTree(
    CScriptBase* node,
    void* root_link);

void __fastcall BinaryTree_InsertDigScriptNode(
    BinaryTreeDigScriptOverlay* self,
    CGameScriptInterfaceBase* scriptInterface,
    CScriptBase** out_node,
    int force_left,
    CScriptBase* parent,
    CCharStringData** key,
    int force_right)
{
    CScriptBase* new_node;
    CScriptBase* root_or_head;

    if (parent == self->root_or_head) {
insert_left:
        new_node = NScript::CExpression_DigScript::Alloc(self, scriptInterface);
        reinterpret_cast<BinaryTreeDigScriptNodeOverlay*>(parent)->left = new_node;

        root_or_head = self->root_or_head;
        if (parent != root_or_head) {
            if (parent == reinterpret_cast<BinaryTreeDigScriptNodeOverlay*>(root_or_head)->left) {
                reinterpret_cast<BinaryTreeDigScriptNodeOverlay*>(root_or_head)->left = new_node;
            }
            goto finish_insert;
        }

        reinterpret_cast<BinaryTreeDigScriptNodeOverlay*>(root_or_head)->parent = new_node;
        root_or_head = self->root_or_head;
    } else {
        if (force_right == 0) {
            if (force_left != 0) {
                goto insert_left;
            }

            CCharStringData* const parent_key =
                reinterpret_cast<BinaryTreeDigScriptNodeOverlay*>(parent)->key;
            CCharStringData* const search_key = *key;

            if (parent_key != search_key) {
                if (search_key == nullptr) {
                    goto insert_left;
                }

                if (parent_key != nullptr) {
                    const long compare_result =
                        CBasicString<char>::Compare(search_key->m_psz, parent_key->m_psz);
                    if (compare_result < 0) {
                        goto insert_left;
                    }
                }
            }
        }

        new_node = NScript::CExpression_DigScript::Alloc(self, scriptInterface);
        reinterpret_cast<BinaryTreeDigScriptNodeOverlay*>(parent)->right = new_node;

        root_or_head = self->root_or_head;
        if (parent != reinterpret_cast<BinaryTreeDigScriptNodeOverlay*>(root_or_head)->right) {
            goto finish_insert;
        }
    }

    reinterpret_cast<BinaryTreeDigScriptNodeOverlay*>(root_or_head)->right = new_node;

finish_insert:
    reinterpret_cast<BinaryTreeDigScriptNodeOverlay*>(new_node)->left = nullptr;
    reinterpret_cast<BinaryTreeDigScriptNodeOverlay*>(new_node)->right = nullptr;
    reinterpret_cast<BinaryTreeDigScriptNodeOverlay*>(new_node)->parent = parent;

    CMemoryAllocatorVariableSize__BalanceTree(
        new_node,
        reinterpret_cast<std::byte*>(self->root_or_head) + 0x04);

    self->count = self->count + 1;
    *out_node = new_node;
}
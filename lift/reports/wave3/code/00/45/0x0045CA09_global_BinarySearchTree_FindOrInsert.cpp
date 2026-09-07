#include <cstddef>
#include <cstdint>

struct BinarySearchTreeNodeOverlay {
    std::byte pad_00[0x08];
    BinarySearchTreeNodeOverlay* left;   // 0x08
    BinarySearchTreeNodeOverlay* right;  // 0x0C
    std::uint32_t key;                   // 0x10
};
static_assert(offsetof(BinarySearchTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(BinarySearchTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(BinarySearchTreeNodeOverlay, key) == 0x10);

struct BinarySearchTreeHeaderOverlay {
    std::byte pad_00[0x04];
    BinarySearchTreeNodeOverlay* root;      // 0x04
    BinarySearchTreeNodeOverlay* sentinel;  // 0x08
};
static_assert(offsetof(BinarySearchTreeHeaderOverlay, root) == 0x04);
static_assert(offsetof(BinarySearchTreeHeaderOverlay, sentinel) == 0x08);

struct BinarySearchTreeOverlay {
    BinarySearchTreeHeaderOverlay* header;  // 0x00
};
static_assert(offsetof(BinarySearchTreeOverlay, header) == 0x00);

struct BinarySearchTreeFindOrInsertResult {
    BinarySearchTreeNodeOverlay* node;  // 0x00
    bool inserted;                      // 0x04
    std::byte pad_05[3];
};
static_assert(offsetof(BinarySearchTreeFindOrInsertResult, node) == 0x00);
static_assert(offsetof(BinarySearchTreeFindOrInsertResult, inserted) == 0x04);
static_assert(sizeof(BinarySearchTreeFindOrInsertResult) == 0x08);

struct BinarySearchTreeInsertResultOverlay {
    BinarySearchTreeNodeOverlay* node;  // 0x00
};
static_assert(offsetof(BinarySearchTreeInsertResultOverlay, node) == 0x00);

extern BinarySearchTreeNodeOverlay* __cdecl RBTree__GetMaxNode(
    BinarySearchTreeNodeOverlay* node);

extern BinarySearchTreeInsertResultOverlay* __thiscall BinarySearchTree_InsertNode(
    BinarySearchTreeOverlay* self,
    std::uint32_t** keyLocal,
    BinarySearchTreeNodeOverlay* insertAt,
    BinarySearchTreeNodeOverlay* parent,
    std::uint32_t* key,
    int zeroFlag);

BinarySearchTreeFindOrInsertResult* __thiscall BinarySearchTree_FindOrInsert(
    BinarySearchTreeOverlay* self,
    BinarySearchTreeFindOrInsertResult* result,
    std::uint32_t* key)
{
    BinarySearchTreeHeaderOverlay* const header = self->header;
    BinarySearchTreeNodeOverlay* current = header->root;
    bool wentLeft = true;
    BinarySearchTreeNodeOverlay* parent =
        reinterpret_cast<BinarySearchTreeNodeOverlay*>(header);

    if (current != nullptr) {
        do {
            parent = current;
            wentLeft = (*key < parent->key);
            if (wentLeft) {
                current = parent->left;
            } else {
                current = parent->right;
            }
        } while (current != nullptr);
    }

    BinarySearchTreeNodeOverlay* candidate = parent;
    BinarySearchTreeNodeOverlay* insertAt;

    if (wentLeft) {
        insertAt = parent;
        if (parent == header->sentinel) {
            goto insert_node;
        }
        candidate = RBTree__GetMaxNode(parent);
    }

    insertAt = current;
    if (*key <= candidate->key) {
        result->node = candidate;
        result->inserted = false;
        return result;
    }

insert_node:
    {
        BinarySearchTreeInsertResultOverlay* const insertResult =
            BinarySearchTree_InsertNode(self, &key, insertAt, parent, key, 0);
        result->node = insertResult->node;
        result->inserted = true;
        return result;
    }
}
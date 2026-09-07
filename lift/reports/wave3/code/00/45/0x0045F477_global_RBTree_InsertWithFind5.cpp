#include <cstddef>
#include <cstdint>

struct RBTreeNode5 {
    std::byte pad00[0x08];
    RBTreeNode5* left;   // 0x08
    RBTreeNode5* right;  // 0x0C
    int key;             // 0x10
};

static_assert(offsetof(RBTreeNode5, left) == 0x08);
static_assert(offsetof(RBTreeNode5, right) == 0x0C);
static_assert(offsetof(RBTreeNode5, key) == 0x10);

struct RBTreeHeader5 {
    std::byte pad00[0x04];
    RBTreeNode5* root;      // 0x04
    RBTreeNode5* leftmost;  // 0x08
};

static_assert(offsetof(RBTreeHeader5, root) == 0x04);
static_assert(offsetof(RBTreeHeader5, leftmost) == 0x08);

struct RBTree5 {
    RBTreeHeader5* header;  // 0x00
};

static_assert(offsetof(RBTree5, header) == 0x00);

struct RBTreeInsertFindResult5 {
    RBTreeNode5* node;  // 0x00
    std::uint8_t inserted; // 0x04
};

static_assert(offsetof(RBTreeInsertFindResult5, node) == 0x00);
static_assert(offsetof(RBTreeInsertFindResult5, inserted) == 0x04);

extern "C" RBTreeNode5* __cdecl RBTree__GetMaxNode(RBTreeNode5* node);
extern "C" RBTreeNode5** __thiscall RBTree_InsertNewNode5(
    RBTree5* self,
    int** key_ref,
    RBTreeNode5* child_hint,
    RBTreeNode5* parent_or_header,
    int* key_ptr,
    int zero
);

RBTreeInsertFindResult5* __thiscall RBTree_InsertWithFind5(
    RBTree5* self,
    RBTreeInsertFindResult5* out_result,
    int* key_ptr
) {
    RBTreeHeader5* const header = self->header;
    RBTreeNode5* search = header->root;
    bool went_left = true;
    RBTreeNode5* current = reinterpret_cast<RBTreeNode5*>(header);

    if (search != nullptr) {
        do {
            current = search;
            went_left = *key_ptr < current->key;
            if (went_left) {
                search = current->left;
            } else {
                search = current->right;
            }
        } while (search != nullptr);
    }

    RBTreeNode5* candidate = current;
    if (went_left) {
        RBTreeNode5* child_hint = current;
        if (current == header->leftmost) {
            RBTreeNode5** const inserted = RBTree_InsertNewNode5(self, &key_ptr, child_hint, current, key_ptr, 0);
            out_result->node = *inserted;
            out_result->inserted = 1;
            return out_result;
        }

        candidate = RBTree__GetMaxNode(current);
    }

    if (*key_ptr <= candidate->key) {
        out_result->node = candidate;
        out_result->inserted = 0;
        return out_result;
    }

    RBTreeNode5* child_hint = search;
    RBTreeNode5** const inserted = RBTree_InsertNewNode5(self, &key_ptr, child_hint, current, key_ptr, 0);
    out_result->node = *inserted;
    out_result->inserted = 1;
    return out_result;
}
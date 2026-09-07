#include <cstddef>
#include <cstdint>

struct RBTreeNode4Overlay {
    std::byte pad_00[0x08];
    RBTreeNode4Overlay* left;   // 0x08
    RBTreeNode4Overlay* right;  // 0x0C
    int key;                    // 0x10
};

static_assert(offsetof(RBTreeNode4Overlay, left) == 0x08);
static_assert(offsetof(RBTreeNode4Overlay, right) == 0x0C);
static_assert(offsetof(RBTreeNode4Overlay, key) == 0x10);

struct RBTreeImpl4Overlay {
    std::byte pad_00[0x04];
    RBTreeNode4Overlay* root;    // 0x04
    RBTreeNode4Overlay* field_8; // 0x08
};

static_assert(offsetof(RBTreeImpl4Overlay, root) == 0x04);
static_assert(offsetof(RBTreeImpl4Overlay, field_8) == 0x08);

struct RBTree4Overlay {
    RBTreeImpl4Overlay* impl; // 0x00
};

static_assert(offsetof(RBTree4Overlay, impl) == 0x00);

struct RBTreeInsertResult4Overlay {
    RBTreeNode4Overlay* node;   // 0x00
    std::uint8_t inserted;      // 0x04
    std::byte pad_05[0x03];
};

static_assert(offsetof(RBTreeInsertResult4Overlay, node) == 0x00);
static_assert(offsetof(RBTreeInsertResult4Overlay, inserted) == 0x04);

extern "C" RBTreeNode4Overlay* RBTree__GetMaxNode(RBTreeNode4Overlay* node);
extern "C" RBTreeInsertResult4Overlay* __thiscall RBTree_InsertNewNode4(
    RBTree4Overlay* self,
    const int** key_slot,
    RBTreeNode4Overlay* child,
    RBTreeNode4Overlay* parent,
    const int* key,
    int zero
);

RBTreeInsertResult4Overlay* __thiscall RBTree_InsertWithFind4(
    RBTree4Overlay* self,
    RBTreeInsertResult4Overlay* result,
    const int* key
) {
    RBTreeImpl4Overlay* const impl = self->impl;
    RBTreeNode4Overlay* current = impl->root;
    bool went_left = true;
    RBTreeNode4Overlay* parent = reinterpret_cast<RBTreeNode4Overlay*>(impl);

    if (current != nullptr) {
        do {
            parent = current;
            went_left = *key < parent->key;
            if (went_left) {
                current = parent->left;
            } else {
                current = parent->right;
            }
        } while (current != nullptr);
    }

    RBTreeNode4Overlay* candidate = parent;
    RBTreeNode4Overlay* child = current;

    if (went_left) {
        child = parent;
        if (parent == impl->field_8) {
            goto insert_new;
        }
        candidate = RBTree__GetMaxNode(parent);
    }

    child = current;
    if (*key <= candidate->key) {
        result->node = candidate;
        result->inserted = 0;
        return result;
    }

insert_new:
    {
        const int* key_slot = key;
        RBTreeInsertResult4Overlay* const inserted_result =
            RBTree_InsertNewNode4(self, &key_slot, child, parent, key, 0);
        result->node = inserted_result->node;
        result->inserted = 1;
        return result;
    }
}
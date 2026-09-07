#include <cstddef>
#include <cstdint>

static_assert(sizeof(void*) == 4);

struct RBTreeNode3 {
    std::byte pad_00[0x08];
    RBTreeNode3* left;   // 0x08
    RBTreeNode3* right;  // 0x0C
    std::int32_t key;    // 0x10
};
static_assert(offsetof(RBTreeNode3, left) == 0x08);
static_assert(offsetof(RBTreeNode3, right) == 0x0C);
static_assert(offsetof(RBTreeNode3, key) == 0x10);

struct RBTreeHead3 {
    std::byte pad_00[0x04];
    RBTreeNode3* root;      // 0x04
    RBTreeNode3* leftmost;  // 0x08
};
static_assert(offsetof(RBTreeHead3, root) == 0x04);
static_assert(offsetof(RBTreeHead3, leftmost) == 0x08);

struct RBTreeOverlay3 {
    RBTreeHead3* head;  // 0x00
};
static_assert(offsetof(RBTreeOverlay3, head) == 0x00);

struct RBTreeInsertFindResult3 {
    RBTreeNode3* node;     // 0x00
    std::uint8_t inserted; // 0x04
    std::byte pad_05[0x03];
};
static_assert(offsetof(RBTreeInsertFindResult3, node) == 0x00);
static_assert(offsetof(RBTreeInsertFindResult3, inserted) == 0x04);

extern RBTreeNode3* __cdecl RBTree__GetMaxNode(RBTreeNode3* node);
extern RBTreeNode3** __thiscall RBTree_InsertNewNode3(
    RBTreeOverlay3* self,
    std::int32_t** value_slot,
    RBTreeNode3* child,
    RBTreeNode3* parent,
    std::int32_t* value,
    int zero
);

RBTreeInsertFindResult3* __thiscall RBTree_InsertWithFind3(
    RBTreeOverlay3* self,
    RBTreeInsertFindResult3* result,
    std::int32_t* value
) {
    RBTreeHead3* const head = self->head;
    RBTreeNode3* current = head->root;
    bool go_left = true;
    RBTreeNode3* parent = reinterpret_cast<RBTreeNode3*>(head);

    if (current != nullptr) {
        do {
            parent = current;
            go_left = *value < parent->key;
            if (go_left) {
                current = parent->left;
            } else {
                current = parent->right;
            }
        } while (current != nullptr);
    }

    RBTreeNode3* candidate = parent;
    if (go_left) {
        RBTreeNode3* insert_child = parent;
        if (parent == head->leftmost) {
            RBTreeNode3* const inserted_node =
                *RBTree_InsertNewNode3(self, &value, insert_child, parent, value, 0);
            result->node = inserted_node;
            result->inserted = 1;
            return result;
        }

        candidate = RBTree__GetMaxNode(parent);
    }

    if (*value <= candidate->key) {
        result->node = candidate;
        result->inserted = 0;
        return result;
    }

    RBTreeNode3* const inserted_node =
        *RBTree_InsertNewNode3(self, &value, current, parent, value, 0);
    result->node = inserted_node;
    result->inserted = 1;
    return result;
}
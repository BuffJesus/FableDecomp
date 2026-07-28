#include <cstddef>
#include <cstdint>

struct BinaryTreeNodeOverlay {
    std::byte pad00[0x08];
    BinaryTreeNodeOverlay* left;   // 0x08
    BinaryTreeNodeOverlay* right;  // 0x0C
    std::uint32_t key;             // 0x10
};

static_assert(offsetof(BinaryTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(BinaryTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(BinaryTreeNodeOverlay, key) == 0x10);

struct BinaryTreeHeadOverlay {
    std::byte pad00[0x04];
    BinaryTreeNodeOverlay* root;      // 0x04
    BinaryTreeNodeOverlay* leftmost;  // 0x08
};

static_assert(offsetof(BinaryTreeHeadOverlay, root) == 0x04);
static_assert(offsetof(BinaryTreeHeadOverlay, leftmost) == 0x08);

struct BinaryTreeOverlay {
    BinaryTreeHeadOverlay* head;  // 0x00
};

static_assert(offsetof(BinaryTreeOverlay, head) == 0x00);

struct BinaryTreeInsertPositionResult {
    BinaryTreeNodeOverlay* node;  // 0x00
    bool inserted;                // 0x04
    std::byte pad05[3];
};

static_assert(offsetof(BinaryTreeInsertPositionResult, node) == 0x00);
static_assert(offsetof(BinaryTreeInsertPositionResult, inserted) == 0x04);

struct StdSetInsertReturnOverlay {
    BinaryTreeNodeOverlay* node;  // 0x00
};

static_assert(offsetof(StdSetInsertReturnOverlay, node) == 0x00);

extern "C" BinaryTreeNodeOverlay* __cdecl RBTree__GetMaxNode(BinaryTreeNodeOverlay* node);

extern "C" StdSetInsertReturnOverlay* __thiscall std_set_insert_uint_via_FUN(
    BinaryTreeOverlay* tree,
    const std::uint32_t** key_slot,
    BinaryTreeNodeOverlay* insert_pos,
    BinaryTreeNodeOverlay* parent,
    const std::uint32_t* key,
    int zero);

BinaryTreeInsertPositionResult* __thiscall BinaryTree_Insert_With_Position(
    BinaryTreeOverlay* tree,
    BinaryTreeInsertPositionResult* out_position,
    const std::uint32_t* key)
{
    BinaryTreeHeadOverlay* const head = tree->head;
    BinaryTreeNodeOverlay* current = head->root;
    bool take_left = true;
    BinaryTreeNodeOverlay* parent = reinterpret_cast<BinaryTreeNodeOverlay*>(head);

    if (current != nullptr) {
        do {
            parent = current;
            take_left = *key < parent->key;
            if (take_left) {
                current = parent->left;
            } else {
                current = parent->right;
            }
        } while (current != nullptr);
    }

    BinaryTreeNodeOverlay* candidate = parent;
    if (take_left) {
        BinaryTreeNodeOverlay* insert_pos = parent;
        if (parent == head->leftmost) {
            const std::uint32_t* key_slot = key;
            StdSetInsertReturnOverlay* const inserted =
                std_set_insert_uint_via_FUN(tree, &key_slot, insert_pos, parent, key, 0);
            out_position->node = inserted->node;
            out_position->inserted = true;
            return out_position;
        }

        candidate = RBTree__GetMaxNode(parent);
    }

    BinaryTreeNodeOverlay* const insert_pos = current;
    if (*key <= candidate->key) {
        out_position->node = candidate;
        out_position->inserted = false;
        return out_position;
    }

    const std::uint32_t* key_slot = key;
    StdSetInsertReturnOverlay* const inserted =
        std_set_insert_uint_via_FUN(tree, &key_slot, insert_pos, parent, key, 0);
    out_position->node = inserted->node;
    out_position->inserted = true;
    return out_position;
}
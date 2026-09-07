#include <cstddef>
#include <cstdint>

struct RBTreeNodeOverlay {
    std::uint8_t pad0[0x08];
    RBTreeNodeOverlay* left;   // 0x08
    RBTreeNodeOverlay* right;  // 0x0C
    int value;                 // 0x10
};

static_assert(offsetof(RBTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, value) == 0x10);

struct RBTreeHeadOverlay {
    std::uint8_t pad0[0x04];
    RBTreeNodeOverlay* root;      // 0x04
    RBTreeNodeOverlay* sentinel;  // 0x08
};

static_assert(offsetof(RBTreeHeadOverlay, root) == 0x04);
static_assert(offsetof(RBTreeHeadOverlay, sentinel) == 0x08);

struct RBTreeOverlay {
    RBTreeHeadOverlay* head;  // 0x00
};

static_assert(offsetof(RBTreeOverlay, head) == 0x00);

struct RBTreeLowerBoundResult {
    RBTreeNodeOverlay* node;  // 0x00
    std::uint8_t inserted;    // 0x04
};

static_assert(offsetof(RBTreeLowerBoundResult, node) == 0x00);
static_assert(offsetof(RBTreeLowerBoundResult, inserted) == 0x04);

extern RBTreeNodeOverlay* __cdecl RBTree__GetMaxNode(RBTreeNodeOverlay* node);
extern RBTreeNodeOverlay** __thiscall RBTree_InsertNode(
    RBTreeOverlay* self,
    int** keyLocal,
    RBTreeNodeOverlay* child,
    RBTreeNodeOverlay* parent,
    int* key,
    int zero
);

RBTreeLowerBoundResult* __thiscall RBTree_LowerBound(
    RBTreeOverlay* self,
    RBTreeLowerBoundResult* result,
    int* key
) {
    RBTreeHeadOverlay* const head = self->head;
    RBTreeNodeOverlay* current = head->root;
    bool wentLeft = true;
    RBTreeNodeOverlay* parent = reinterpret_cast<RBTreeNodeOverlay*>(head);

    if (current != nullptr) {
        do {
            parent = current;
            wentLeft = *key < parent->value;
            if (wentLeft) {
                current = parent->left;
            } else {
                current = parent->right;
            }
        } while (current != nullptr);
    }

    RBTreeNodeOverlay* candidate = parent;
    if (wentLeft) {
        RBTreeNodeOverlay* child = parent;
        if (parent == head->sentinel) {
            RBTreeNodeOverlay** const insertedNode =
                RBTree_InsertNode(self, &key, child, parent, key, 0);
            result->node = *insertedNode;
            result->inserted = 1;
            return result;
        }
        candidate = RBTree__GetMaxNode(parent);
    }

    RBTreeNodeOverlay* child = current;
    if (*key <= candidate->value) {
        result->node = candidate;
        result->inserted = 0;
        return result;
    }

    RBTreeNodeOverlay** const insertedNode =
        RBTree_InsertNode(self, &key, child, parent, key, 0);
    result->node = *insertedNode;
    result->inserted = 1;
    return result;
}
#include <cstddef>
#include <cstdint>

struct RBTreeHeaderOverlay {
    std::byte pad_00[0x04];
    std::int32_t root;              // +0x04
    std::int32_t sentinelOrHead;    // +0x08
};
static_assert(offsetof(RBTreeHeaderOverlay, root) == 0x04);
static_assert(offsetof(RBTreeHeaderOverlay, sentinelOrHead) == 0x08);

struct RBTreeNodeOverlay {
    std::byte pad_00[0x08];
    std::int32_t left;              // +0x08
    std::int32_t right;             // +0x0C
    std::int32_t key;               // +0x10
};
static_assert(offsetof(RBTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, key) == 0x10);

struct RBTreeInsertResultOverlay {
    std::int32_t node;              // +0x00
    std::uint8_t insertedRight;     // +0x04
};
static_assert(offsetof(RBTreeInsertResultOverlay, node) == 0x00);
static_assert(offsetof(RBTreeInsertResultOverlay, insertedRight) == 0x04);

struct RBTreeThisOverlay {
    std::int32_t treeValue;         // *param_1
};
static_assert(offsetof(RBTreeThisOverlay, treeValue) == 0x00);

extern "C" std::int32_t __cdecl RBTree__GetMaxNode(std::int32_t node);

struct BinaryTreeThisOverlay {
    std::int32_t* __thiscall InsertNodeRight(
        std::int32_t** nodeRef,
        std::int32_t child,
        std::int32_t parent,
        std::int32_t* node,
        int zeroFlag);
};

extern "C" RBTreeInsertResultOverlay* __thiscall RBTree_InsertRight(
    RBTreeThisOverlay* self,
    RBTreeInsertResultOverlay* result,
    std::int32_t* node)
{
    const std::int32_t treeValue = self->treeValue;
    const auto* const tree =
        reinterpret_cast<const RBTreeHeaderOverlay*>(static_cast<std::uintptr_t>(treeValue));

    std::int32_t current = tree->root;
    bool goLeft = true;
    std::int32_t parent = treeValue;

    if (current != 0) {
        do {
            parent = current;
            goLeft =
                *node <
                reinterpret_cast<const RBTreeNodeOverlay*>(static_cast<std::uintptr_t>(parent))->key;
            if (goLeft) {
                current = reinterpret_cast<const RBTreeNodeOverlay*>(
                              static_cast<std::uintptr_t>(parent))
                              ->left;
            } else {
                current = reinterpret_cast<const RBTreeNodeOverlay*>(
                              static_cast<std::uintptr_t>(parent))
                              ->right;
            }
        } while (current != 0);
    }

    std::int32_t compareNode = parent;
    std::int32_t child;

    if (goLeft) {
        child = parent;
        if (parent == tree->sentinelOrHead) {
            goto insert_right;
        }
        compareNode = RBTree__GetMaxNode(parent);
    }

    child = current;
    if (*node <= reinterpret_cast<const RBTreeNodeOverlay*>(
                     static_cast<std::uintptr_t>(compareNode))
                     ->key) {
        result->node = compareNode;
        result->insertedRight = 0;
        return result;
    }

insert_right:
    {
        std::int32_t* nodeLocal = node;
        std::int32_t* const inserted =
            reinterpret_cast<BinaryTreeThisOverlay*>(self)->InsertNodeRight(
                &nodeLocal,
                child,
                parent,
                node,
                0);
        result->node = *inserted;
        result->insertedRight = 1;
        return result;
    }
}
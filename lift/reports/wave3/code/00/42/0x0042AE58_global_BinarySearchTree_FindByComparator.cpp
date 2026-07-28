#include <cstddef>
#include <functional>

struct CCharString;

struct BinarySearchTreeNodeOverlay {
    std::byte _pad00[0x08];
    int child_at_0x08;
    int child_at_0x0C;
    CCharString key;
};

static_assert(offsetof(BinarySearchTreeNodeOverlay, child_at_0x08) == 0x08);
static_assert(offsetof(BinarySearchTreeNodeOverlay, child_at_0x0C) == 0x0C);
static_assert(offsetof(BinarySearchTreeNodeOverlay, key) == 0x10);

struct BinarySearchTreeHeaderOverlay {
    int field_0x00;
    int root;
};

static_assert(offsetof(BinarySearchTreeHeaderOverlay, root) == 0x04);

struct BinarySearchTreeOverlay {
    BinarySearchTreeHeaderOverlay* header;
    std::byte _pad04[0x04];
    std::less<CCharString> comparator;
};

static_assert(offsetof(BinarySearchTreeOverlay, comparator) == 0x08);

int __thiscall BinarySearchTree_FindByComparator(BinarySearchTreeOverlay* self, CCharString* value)
{
    int current_best = reinterpret_cast<int>(self->header);

    if (self->header->root != 0) {
        int node_ptr = self->header->root;

        do {
            const bool go_to_child_0x0C =
                self->comparator(reinterpret_cast<BinarySearchTreeNodeOverlay*>(node_ptr)->key, *value);

            int next_node;
            if (go_to_child_0x0C) {
                next_node = reinterpret_cast<BinarySearchTreeNodeOverlay*>(node_ptr)->child_at_0x0C;
            } else {
                next_node = reinterpret_cast<BinarySearchTreeNodeOverlay*>(node_ptr)->child_at_0x08;
                current_best = node_ptr;
            }

            node_ptr = next_node;
        } while (next_node != 0);
    }

    return current_best;
}
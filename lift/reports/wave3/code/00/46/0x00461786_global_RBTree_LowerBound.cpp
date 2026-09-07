#include <cstddef>
#include <cstdint>

static_assert(sizeof(void*) == 4, "RBTree_LowerBound is a 32-bit x86 function.");

struct RBTreeNodeOverlay {
    std::byte pad_00[0x08];
    RBTreeNodeOverlay* left;
    RBTreeNodeOverlay* right;
    int value;
};
static_assert(offsetof(RBTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, value) == 0x10);

struct RBTreeHeaderOverlay {
    std::byte pad_00[0x04];
    RBTreeNodeOverlay* field_04;
    RBTreeNodeOverlay* field_08;
};
static_assert(offsetof(RBTreeHeaderOverlay, field_04) == 0x04);
static_assert(offsetof(RBTreeHeaderOverlay, field_08) == 0x08);

struct RBTreeOverlay {
    RBTreeHeaderOverlay* header;
};
static_assert(offsetof(RBTreeOverlay, header) == 0x00);

struct RBTreeLowerBoundResultOverlay {
    RBTreeNodeOverlay* node;
    bool inserted;
};
static_assert(offsetof(RBTreeLowerBoundResultOverlay, node) == 0x00);
static_assert(offsetof(RBTreeLowerBoundResultOverlay, inserted) == 0x04);

RBTreeNodeOverlay* __cdecl RBTree__GetMaxNode(RBTreeNodeOverlay* node);
RBTreeNodeOverlay** __thiscall RBTree_Insert_Internal(
    RBTreeOverlay* self,
    int** key_slot,
    RBTreeNodeOverlay* link,
    RBTreeNodeOverlay* parent,
    int* key,
    int zero
);

RBTreeLowerBoundResultOverlay* __thiscall RBTree_LowerBound(
    RBTreeOverlay* self,
    RBTreeLowerBoundResultOverlay* result,
    int* key
) {
    RBTreeHeaderOverlay* const header = self->header;
    RBTreeNodeOverlay* link = header->field_04;
    bool is_less = true;
    RBTreeNodeOverlay* parent = reinterpret_cast<RBTreeNodeOverlay*>(header);

    if (link != nullptr) {
        do {
            parent = link;
            is_less = *key < parent->value;
            if (is_less) {
                link = parent->left;
            } else {
                link = parent->right;
            }
        } while (link != nullptr);
    }

    RBTreeNodeOverlay* candidate = parent;
    if (is_less) {
        RBTreeNodeOverlay* const insert_link = parent;
        if (parent == header->field_08) {
            RBTreeNodeOverlay** const inserted =
                RBTree_Insert_Internal(self, &key, insert_link, parent, key, 0);
            result->node = *inserted;
            result->inserted = true;
            return result;
        }

        candidate = RBTree__GetMaxNode(parent);
    }

    if (*key <= candidate->value) {
        result->node = candidate;
        result->inserted = false;
        return result;
    }

    RBTreeNodeOverlay** const inserted =
        RBTree_Insert_Internal(self, &key, link, parent, key, 0);
    result->node = *inserted;
    result->inserted = true;
    return result;
}
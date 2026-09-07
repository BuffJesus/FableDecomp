#include <cstddef>
#include <cstdint>

struct RBTreeNodeOverlay {
    std::uint8_t pad_00[0x08];
    RBTreeNodeOverlay* left;   // 0x08
    RBTreeNodeOverlay* right;  // 0x0C
    int key;                   // 0x10
};

static_assert(offsetof(RBTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, key) == 0x10);

struct RBTreeHeaderOverlay {
    std::uint8_t pad_00[0x04];
    RBTreeNodeOverlay* root;        // 0x04
    RBTreeNodeOverlay* bound_node;  // 0x08
};

static_assert(offsetof(RBTreeHeaderOverlay, root) == 0x04);
static_assert(offsetof(RBTreeHeaderOverlay, bound_node) == 0x08);

struct RBTreeOverlay {
    RBTreeHeaderOverlay* header; // 0x00
};

static_assert(offsetof(RBTreeOverlay, header) == 0x00);

struct RBTreeLowerBoundResultOverlay {
    RBTreeNodeOverlay* node;   // 0x00
    std::uint8_t inserted;     // 0x04
};

static_assert(offsetof(RBTreeLowerBoundResultOverlay, node) == 0x00);
static_assert(offsetof(RBTreeLowerBoundResultOverlay, inserted) == 0x04);

extern "C" RBTreeNodeOverlay* __cdecl RBTree__GetMaxNode(RBTreeNodeOverlay* node);
extern "C" RBTreeLowerBoundResultOverlay* __thiscall RBTree_InsertNode(
    RBTreeOverlay* self,
    int** key_ref,
    RBTreeNodeOverlay* a3,
    RBTreeNodeOverlay* a4,
    int* key,
    int a6);

RBTreeLowerBoundResultOverlay* __thiscall RBTree_LowerBound(
    RBTreeOverlay* self,
    RBTreeLowerBoundResultOverlay* out_result,
    int* key)
{
    RBTreeHeaderOverlay* const header = self->header;
    RBTreeNodeOverlay* current = header->root;
    bool went_left = true;
    RBTreeNodeOverlay* last = reinterpret_cast<RBTreeNodeOverlay*>(header);

    if (current != nullptr) {
        do {
            last = current;
            went_left = *key < last->key;
            current = went_left ? last->left : last->right;
        } while (current != nullptr);
    }

    RBTreeNodeOverlay* candidate = last;
    RBTreeNodeOverlay* insert_hint;

    if (went_left) {
        insert_hint = last;
        if (last == header->bound_node) {
            RBTreeLowerBoundResultOverlay* const insert_result =
                RBTree_InsertNode(self, &key, insert_hint, last, key, 0);
            out_result->node = insert_result->node;
            out_result->inserted = 1;
            return out_result;
        }

        candidate = RBTree__GetMaxNode(last);
    }

    insert_hint = current;
    if (*key <= candidate->key) {
        out_result->node = candidate;
        out_result->inserted = 0;
        return out_result;
    }

    RBTreeLowerBoundResultOverlay* const insert_result =
        RBTree_InsertNode(self, &key, insert_hint, last, key, 0);
    out_result->node = insert_result->node;
    out_result->inserted = 1;
    return out_result;
}
#include <cstddef>
#include <cstdint>

struct RBTreeNodeOverlay {
    std::byte field_0x0[0x8];
    RBTreeNodeOverlay* field_0x8;
    RBTreeNodeOverlay* field_0xC;
    int field_0x10;
};

static_assert(offsetof(RBTreeNodeOverlay, field_0x8) == 0x8);
static_assert(offsetof(RBTreeNodeOverlay, field_0xC) == 0xC);
static_assert(offsetof(RBTreeNodeOverlay, field_0x10) == 0x10);

struct RBTreeHeaderOverlay {
    std::byte field_0x0[0x4];
    RBTreeNodeOverlay* field_0x4;
    RBTreeNodeOverlay* field_0x8;
};

static_assert(offsetof(RBTreeHeaderOverlay, field_0x4) == 0x4);
static_assert(offsetof(RBTreeHeaderOverlay, field_0x8) == 0x8);

struct RBTreeInsertFindResultOverlay {
    RBTreeNodeOverlay* node;
    std::uint8_t inserted;
};

static_assert(offsetof(RBTreeInsertFindResultOverlay, node) == 0x0);
static_assert(offsetof(RBTreeInsertFindResultOverlay, inserted) == 0x4);

extern RBTreeNodeOverlay* __cdecl RBTree__GetMaxNode(RBTreeNodeOverlay* node);
extern RBTreeNodeOverlay** __thiscall RBTree_InsertNewNode2(
    int** key_indirect,
    RBTreeNodeOverlay* parent_for_insert,
    RBTreeNodeOverlay* search_base,
    int* key_ptr,
    int zero
);

RBTreeInsertFindResultOverlay* __thiscall RBTree_InsertWithFind2(
    RBTreeHeaderOverlay** this_ptr,
    RBTreeInsertFindResultOverlay* out_result,
    int* key_ptr
) {
    RBTreeHeaderOverlay* const tree = *this_ptr;
    RBTreeNodeOverlay* current = tree->field_0x4;
    bool went_left = true;
    RBTreeNodeOverlay* search_base = reinterpret_cast<RBTreeNodeOverlay*>(tree);

    if (current != nullptr) {
        do {
            search_base = current;
            went_left = *key_ptr < search_base->field_0x10;
            if (went_left) {
                current = search_base->field_0x8;
            } else {
                current = search_base->field_0xC;
            }
        } while (current != nullptr);
    }

    RBTreeNodeOverlay* candidate = search_base;
    RBTreeNodeOverlay* parent_for_insert;

    if (went_left) {
        parent_for_insert = search_base;
        if (search_base == tree->field_0x8) {
            goto insert_new_node;
        }
        candidate = RBTree__GetMaxNode(search_base);
    }

    parent_for_insert = current;
    if (*key_ptr <= candidate->field_0x10) {
        out_result->node = candidate;
        out_result->inserted = 0;
        return out_result;
    }

insert_new_node:
    {
        RBTreeNodeOverlay** const inserted_node_slot =
            RBTree_InsertNewNode2(&key_ptr, parent_for_insert, search_base, key_ptr, 0);
        out_result->node = *inserted_node_slot;
        out_result->inserted = 1;
        return out_result;
    }
}
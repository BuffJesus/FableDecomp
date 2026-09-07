#include <cstddef>
#include <cstdint>

struct RBTreeNodeOverlay {
    std::byte pad00[0x08];
    RBTreeNodeOverlay* left;   // 0x08
    RBTreeNodeOverlay* right;  // 0x0C
    std::int32_t key;          // 0x10
};

static_assert(offsetof(RBTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, key) == 0x10);

struct RBTreeHeaderOverlay {
    RBTreeNodeOverlay* unknown00; // 0x00
    RBTreeNodeOverlay* root;      // 0x04
    RBTreeNodeOverlay* bound08;   // 0x08
};

static_assert(offsetof(RBTreeHeaderOverlay, root) == 0x04);
static_assert(offsetof(RBTreeHeaderOverlay, bound08) == 0x08);

struct RBTreeOverlay {
    RBTreeHeaderOverlay* header; // 0x00
};

static_assert(offsetof(RBTreeOverlay, header) == 0x00);

struct RBTreeLowerBoundResultOverlay {
    RBTreeNodeOverlay* node; // 0x00
    std::uint8_t inserted;   // 0x04
};

static_assert(offsetof(RBTreeLowerBoundResultOverlay, node) == 0x00);
static_assert(offsetof(RBTreeLowerBoundResultOverlay, inserted) == 0x04);

extern "C" RBTreeNodeOverlay* __cdecl RBTree__GetMaxNode(RBTreeNodeOverlay* node);
extern "C" RBTreeNodeOverlay** __thiscall RBTree_Insert_Internal(
    RBTreeOverlay* self,
    const std::int32_t** key_local,
    RBTreeNodeOverlay* arg2,
    RBTreeNodeOverlay* arg3,
    const std::int32_t* key,
    int zero);

RBTreeLowerBoundResultOverlay* __thiscall RBTree_LowerBound(
    RBTreeOverlay* self,
    RBTreeLowerBoundResultOverlay* result,
    const std::int32_t* key_ptr)
{
    RBTreeHeaderOverlay* const header = self->header;
    RBTreeNodeOverlay* current = header->root;
    bool took_left = true;
    RBTreeNodeOverlay* last = reinterpret_cast<RBTreeNodeOverlay*>(header);

    if (current != nullptr) {
        do {
            last = current;
            took_left = *key_ptr < last->key;
            if (took_left) {
                current = last->left;
            } else {
                current = last->right;
            }
        } while (current != nullptr);
    }

    RBTreeNodeOverlay* candidate = last;
    RBTreeNodeOverlay* insert_hint = current;

    if (took_left) {
        insert_hint = last;
        if (last == header->bound08) {
            const std::int32_t* key_local = key_ptr;
            RBTreeNodeOverlay** const inserted_node =
                RBTree_Insert_Internal(self, &key_local, insert_hint, last, key_ptr, 0);
            result->node = *inserted_node;
            result->inserted = 1;
            return result;
        }
        candidate = RBTree__GetMaxNode(last);
    }

    if (*key_ptr <= candidate->key) {
        result->node = candidate;
        result->inserted = 0;
        return result;
    }

    {
        const std::int32_t* key_local = key_ptr;
        RBTreeNodeOverlay** const inserted_node =
            RBTree_Insert_Internal(self, &key_local, insert_hint, last, key_ptr, 0);
        result->node = *inserted_node;
        result->inserted = 1;
        return result;
    }
}
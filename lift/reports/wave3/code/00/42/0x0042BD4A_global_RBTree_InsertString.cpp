#include <cstddef>
#include <cstdint>
#include <functional>

struct CCharString;

struct RBTreeInsertStringResultOverlay {
    std::uint32_t node;
    bool inserted;
    std::byte padding[3];
};
static_assert(offsetof(RBTreeInsertStringResultOverlay, node) == 0x0);
static_assert(offsetof(RBTreeInsertStringResultOverlay, inserted) == 0x4);

struct RBTreeStringNodeOverlay {
    std::uint32_t reserved0;
    std::uint32_t reserved4;
    std::uint32_t left;
    std::uint32_t right;
};
static_assert(offsetof(RBTreeStringNodeOverlay, left) == 0x8);
static_assert(offsetof(RBTreeStringNodeOverlay, right) == 0xC);

struct RBTreeStringHeaderOverlay {
    std::uint32_t reserved0;
    std::uint32_t root;
    std::uint32_t leftmost;
};
static_assert(offsetof(RBTreeStringHeaderOverlay, root) == 0x4);
static_assert(offsetof(RBTreeStringHeaderOverlay, leftmost) == 0x8);

struct RBTreeStringOverlay {
    std::uint32_t header;
    std::byte padding4[4];
    std::less<CCharString> less;
};
static_assert(offsetof(RBTreeStringOverlay, header) == 0x0);
static_assert(offsetof(RBTreeStringOverlay, less) == 0x8);

extern std::uint32_t __cdecl RBTree__GetMaxNode(std::uint32_t node);
extern std::uint32_t* __thiscall CMemoryAllocatorVariableSize_InsertNode(
    RBTreeStringOverlay* self,
    CCharString** value_slot,
    std::uint32_t attach_point,
    std::uint32_t parent,
    CCharString* value,
    int zero
);

static inline CCharString* RBTreeStringNodeValue(std::uint32_t node_addr) {
    return reinterpret_cast<CCharString*>(node_addr + 0x10);
}

void __thiscall RBTree_InsertString(
    RBTreeStringOverlay* self,
    RBTreeInsertStringResultOverlay* result,
    CCharString* value
) {
    const std::uint32_t header_addr = self->header;
    auto* const header = reinterpret_cast<RBTreeStringHeaderOverlay*>(header_addr);

    bool insert_left = true;
    std::uint32_t parent = header_addr;
    std::uint32_t current = header->root;

    while (current != 0) {
        insert_left = self->less(*value, *RBTreeStringNodeValue(current));
        parent = current;

        const auto* const node = reinterpret_cast<const RBTreeStringNodeOverlay*>(current);
        if (insert_left) {
            current = node->left;
        } else {
            current = node->right;
        }
    }

    std::uint32_t candidate = parent;
    std::uint32_t attach_point;

    if (!insert_left) {
compare_against_candidate:
        insert_left = self->less(*RBTreeStringNodeValue(candidate), *value);
        attach_point = current;
        if (!insert_left) {
            result->inserted = false;
            result->node = candidate;
            return;
        }
    } else {
        attach_point = parent;
        if (parent != header->leftmost) {
            candidate = RBTree__GetMaxNode(parent);
            goto compare_against_candidate;
        }
    }

    std::uint32_t* const inserted_node =
        CMemoryAllocatorVariableSize_InsertNode(self, &value, attach_point, parent, value, 0);

    result->inserted = true;
    result->node = *inserted_node;
}
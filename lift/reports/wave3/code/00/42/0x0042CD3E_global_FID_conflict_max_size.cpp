#include <cstddef>
#include <cstdint>

struct FID_conflict_NodeOverlay {
    std::byte _pad0[0x18];
    std::uint32_t value_at_18;
};
static_assert(offsetof(FID_conflict_NodeOverlay, value_at_18) == 0x18);

struct FID_conflict_TreeOverlay {
    FID_conflict_NodeOverlay* sentinel_or_head;
};
static_assert(offsetof(FID_conflict_TreeOverlay, sentinel_or_head) == 0x0);

struct FID_conflict_Overlay {
    std::byte _pad0[0x150];
    FID_conflict_TreeOverlay tree;
};
static_assert(offsetof(FID_conflict_Overlay, tree) == 0x150);

extern "C" FID_conflict_NodeOverlay* __thiscall
BinarySearchTree_FindLessOrEqual_Copy(FID_conflict_TreeOverlay* tree, const void* key);

std::uint32_t __fastcall FID_conflict_max_size(FID_conflict_Overlay* self, void*, std::uint32_t value)
{
    FID_conflict_NodeOverlay* node =
        BinarySearchTree_FindLessOrEqual_Copy(&self->tree, &value);

    if (node == self->tree.sentinel_or_head) {
        return 0xFFFFFFFFu;
    }

    return node->value_at_18;
}
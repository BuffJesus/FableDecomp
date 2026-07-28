#include <cstddef>
#include <cstdint>

struct FID_conflict_max_size_Overlay {
    std::uint8_t pad_0000[0x144];
    void* tree_header;
};

static_assert(offsetof(FID_conflict_max_size_Overlay, tree_header) == 0x144);

extern "C" void* __thiscall BinarySearchTree_FindLessOrEqual_Copy(void* tree, const void* key);

std::uint32_t __thiscall FID_conflict_max_size(FID_conflict_max_size_Overlay* self, const void* key)
{
    void* const node = BinarySearchTree_FindLessOrEqual_Copy(
        reinterpret_cast<void*>(reinterpret_cast<std::uint8_t*>(self) + 0x144),
        key);

    if (node == self->tree_header) {
        return 0xFFFFFFFFu;
    }

    return *reinterpret_cast<const std::uint32_t*>(
        reinterpret_cast<const std::uint8_t*>(node) + 0x18);
}
#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct BinarySearchTreeNode_CreateCopy_Overlay {
    std::byte pad_0x00[0x10];
    std::uint32_t field_0x10;
    std::uint32_t field_0x14;
};

static_assert(offsetof(BinarySearchTreeNode_CreateCopy_Overlay, field_0x10) == 0x10);
static_assert(offsetof(BinarySearchTreeNode_CreateCopy_Overlay, field_0x14) == 0x14);
static_assert(sizeof(BinarySearchTreeNode_CreateCopy_Overlay) == 0x18);

void __cdecl BinarySearchTreeNode_CreateCopy(std::uint32_t* param_1)
{
    void* pvVar1 = std::malloc(0x18);

    if (reinterpret_cast<BinarySearchTreeNode_CreateCopy_Overlay*>(
            reinterpret_cast<std::uintptr_t>(pvVar1) + 0x10) != nullptr) {
        auto* node = reinterpret_cast<BinarySearchTreeNode_CreateCopy_Overlay*>(pvVar1);
        node->field_0x10 = param_1[0];
        node->field_0x14 = param_1[1];
    }
}
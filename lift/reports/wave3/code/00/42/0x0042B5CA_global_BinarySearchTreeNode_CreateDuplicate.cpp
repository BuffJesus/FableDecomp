#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct BinarySearchTreeNodeCreateDuplicate_Overlay {
    std::byte pad_00[0x10];
    std::uint32_t value_10;
    std::uint32_t value_14;
};

static_assert(offsetof(BinarySearchTreeNodeCreateDuplicate_Overlay, value_10) == 0x10);
static_assert(offsetof(BinarySearchTreeNodeCreateDuplicate_Overlay, value_14) == 0x14);
static_assert(sizeof(BinarySearchTreeNodeCreateDuplicate_Overlay) == 0x18);

void BinarySearchTreeNode_CreateDuplicate(std::uint32_t* param_1)
{
    void* pvVar1 = std::malloc(0x18);

    if ((reinterpret_cast<std::uintptr_t>(pvVar1) + 0x10U) != 0) {
        auto* node = reinterpret_cast<BinarySearchTreeNodeCreateDuplicate_Overlay*>(pvVar1);
        node->value_10 = param_1[0];
        node->value_14 = param_1[1];
    }
}
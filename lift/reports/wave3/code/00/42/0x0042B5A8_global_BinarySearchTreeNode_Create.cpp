#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct BinarySearchTreeNodeCreate_Overlay {
    std::byte _pad0[0x10];
    std::uint32_t value0;
    std::uint32_t value1;
};

static_assert(sizeof(BinarySearchTreeNodeCreate_Overlay) == 0x18);
static_assert(offsetof(BinarySearchTreeNodeCreate_Overlay, value0) == 0x10);
static_assert(offsetof(BinarySearchTreeNodeCreate_Overlay, value1) == 0x14);

void __cdecl BinarySearchTreeNode_Create(std::uint32_t* param_1)
{
    void* pvVar1 = std::malloc(0x18);

    auto* const slot = reinterpret_cast<std::uint32_t*>(
        static_cast<std::uintptr_t>(reinterpret_cast<std::uintptr_t>(pvVar1)) + 0x10
    );

    if (slot != nullptr) {
        slot[0] = param_1[0];
        slot[1] = param_1[1];
    }
}
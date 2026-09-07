#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct TreeNodeAllocDataOverlay {
    std::byte pad_00[0x10];
    std::uint32_t value_10;
    std::uint32_t value_14;
};

static_assert(sizeof(TreeNodeAllocDataOverlay) == 0x18);
static_assert(offsetof(TreeNodeAllocDataOverlay, value_10) == 0x10);
static_assert(offsetof(TreeNodeAllocDataOverlay, value_14) == 0x14);

void __cdecl TreeNode_AllocData(std::uint32_t* param_1)
{
    void* const pvVar1 = std::malloc(0x18);

    auto* const slot = reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(pvVar1) + 0x10);
    if (slot != nullptr) {
        slot[0] = param_1[0];
        slot[1] = param_1[1];
    }
}
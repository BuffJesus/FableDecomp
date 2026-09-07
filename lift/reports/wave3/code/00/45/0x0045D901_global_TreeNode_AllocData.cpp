#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct TreeNodeAllocDataOverlay {
    std::byte padding_0x00[0x10];
    std::uint32_t dword_0x10;
    std::uint32_t dword_0x14;
};

static_assert(offsetof(TreeNodeAllocDataOverlay, dword_0x10) == 0x10);
static_assert(offsetof(TreeNodeAllocDataOverlay, dword_0x14) == 0x14);
static_assert(sizeof(TreeNodeAllocDataOverlay) == 0x18);

void __stdcall TreeNode_AllocData(std::uint32_t* param_1)
{
    void* const pvVar1 = std::malloc(0x18);

    if ((reinterpret_cast<std::uintptr_t>(pvVar1) + 0x10) != 0) {
        auto* const data = reinterpret_cast<TreeNodeAllocDataOverlay*>(pvVar1);
        data->dword_0x10 = param_1[0];
        data->dword_0x14 = param_1[1];
    }
}
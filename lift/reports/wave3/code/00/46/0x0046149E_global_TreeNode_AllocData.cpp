#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct TreeNodeAllocDataOverlay
{
    std::byte pad_00[0x10];
    std::uint32_t dword_10;
    std::uint32_t dword_14;
};

static_assert(offsetof(TreeNodeAllocDataOverlay, dword_10) == 0x10);
static_assert(offsetof(TreeNodeAllocDataOverlay, dword_14) == 0x14);
static_assert(sizeof(TreeNodeAllocDataOverlay) == 0x18);

void __stdcall TreeNode_AllocData(std::uint32_t* param_1)
{
    void* const pvVar1 = std::malloc(0x18);

    const std::uintptr_t addr = reinterpret_cast<std::uintptr_t>(pvVar1) + 0x10;
    if (addr != 0) {
        auto* const out = reinterpret_cast<std::uint32_t*>(addr);
        out[0] = param_1[0];
        out[1] = param_1[1];
    }
}
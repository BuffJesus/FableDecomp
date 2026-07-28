#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct TreeNodeAllocDataOverlay {
    std::byte pad_00[0x10];
    std::uint32_t dword_10;
    std::uint32_t dword_14;
};

static_assert(offsetof(TreeNodeAllocDataOverlay, dword_10) == 0x10);
static_assert(offsetof(TreeNodeAllocDataOverlay, dword_14) == 0x14);
static_assert(sizeof(TreeNodeAllocDataOverlay) == 0x18);

void __cdecl TreeNode_AllocData(std::uint32_t* param_1)
{
    void* pvVar1 = std::malloc(0x18);

    auto* const field_10 = reinterpret_cast<std::uint32_t*>(
        reinterpret_cast<std::uintptr_t>(pvVar1) + 0x10
    );

    if (field_10 != nullptr) {
        field_10[0] = param_1[0];
        field_10[1] = param_1[1];
    }
}
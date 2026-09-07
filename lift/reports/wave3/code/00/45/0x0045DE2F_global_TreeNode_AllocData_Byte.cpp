#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct TreeNodeAllocDataByteSource {
    std::uint32_t dword_0x0;
    std::uint8_t byte_0x4;
};

static_assert(offsetof(TreeNodeAllocDataByteSource, dword_0x0) == 0x0);
static_assert(offsetof(TreeNodeAllocDataByteSource, byte_0x4) == 0x4);

struct TreeNodeAllocDataByteBlock {
    std::byte pad_0x00[0x10];
    std::uint32_t dword_0x10;
    std::uint8_t byte_0x14;
    std::byte pad_0x15[0x03];
};

static_assert(offsetof(TreeNodeAllocDataByteBlock, dword_0x10) == 0x10);
static_assert(offsetof(TreeNodeAllocDataByteBlock, byte_0x14) == 0x14);
static_assert(sizeof(TreeNodeAllocDataByteBlock) == 0x18);

void __cdecl TreeNode_AllocData_Byte(TreeNodeAllocDataByteSource* param_1)
{
    void* const pvVar1 = std::malloc(0x18);

    auto* const block = reinterpret_cast<TreeNodeAllocDataByteBlock*>(
        reinterpret_cast<std::uintptr_t>(pvVar1));

    if (reinterpret_cast<TreeNodeAllocDataByteBlock*>(
            reinterpret_cast<std::uintptr_t>(block) + 0x10) != nullptr) {
        block->dword_0x10 = param_1->dword_0x0;
        block->byte_0x14 = param_1->byte_0x4;
    }
}
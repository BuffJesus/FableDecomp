#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct TreeNode_AllocData_Byte_Source
{
    std::uint32_t dword0;
    std::uint8_t byte4;
};

static_assert(offsetof(TreeNode_AllocData_Byte_Source, dword0) == 0x0);
static_assert(offsetof(TreeNode_AllocData_Byte_Source, byte4) == 0x4);

struct TreeNode_AllocData_Byte_Buffer
{
    std::byte pad_00[0x10];
    std::uint32_t dword10;
    std::uint8_t byte14;
};

static_assert(offsetof(TreeNode_AllocData_Byte_Buffer, dword10) == 0x10);
static_assert(offsetof(TreeNode_AllocData_Byte_Buffer, byte14) == 0x14);

void __stdcall TreeNode_AllocData_Byte(const TreeNode_AllocData_Byte_Source* param_1)
{
    void* const pvVar1 = std::malloc(0x18);

    if (reinterpret_cast<std::uintptr_t>(pvVar1) + 0x10 != 0) {
        auto* const buffer = static_cast<TreeNode_AllocData_Byte_Buffer*>(pvVar1);
        buffer->dword10 = param_1->dword0;
        buffer->byte14 = param_1->byte4;
    }
}
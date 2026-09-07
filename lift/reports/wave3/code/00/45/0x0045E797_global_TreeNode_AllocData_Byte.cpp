#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct TreeNodeAllocDataByteSource
{
    std::uint32_t value_0x00;
    std::uint8_t value_0x04;
};

static_assert(offsetof(TreeNodeAllocDataByteSource, value_0x00) == 0x00);
static_assert(offsetof(TreeNodeAllocDataByteSource, value_0x04) == 0x04);

struct TreeNodeAllocDataByteStorage
{
    std::uint8_t pad_0x00[0x10];
    std::uint32_t value_0x10;
    std::uint8_t value_0x14;
    std::uint8_t pad_0x15[0x03];
};

static_assert(offsetof(TreeNodeAllocDataByteStorage, value_0x10) == 0x10);
static_assert(offsetof(TreeNodeAllocDataByteStorage, value_0x14) == 0x14);
static_assert(sizeof(TreeNodeAllocDataByteStorage) == 0x18);

void __stdcall TreeNode_AllocData_Byte(const TreeNodeAllocDataByteSource* param_1)
{
    void* const pvVar1 = std::malloc(0x18);
    auto* const storage = static_cast<TreeNodeAllocDataByteStorage*>(pvVar1);

    if (static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(pvVar1)) + 0x10u != 0) {
        storage->value_0x10 = param_1->value_0x00;
        storage->value_0x14 = param_1->value_0x04;
    }
}
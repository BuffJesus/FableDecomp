#include <cstdint>
#include <cstdlib>

void TreeNode_AllocData(std::uint32_t* param_1)
{
    void* const allocation = std::malloc(0x18);
    const std::uintptr_t data_addr = reinterpret_cast<std::uintptr_t>(allocation) + 0x10;

    if (data_addr != 0) {
        *reinterpret_cast<std::uint32_t*>(data_addr) = param_1[0];
        *reinterpret_cast<std::uint32_t*>(data_addr + 0x4) = param_1[1];
    }
}
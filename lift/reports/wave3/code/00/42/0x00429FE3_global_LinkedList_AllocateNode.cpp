#include <cstdint>
#include <cstdlib>

void __stdcall LinkedList_AllocateNode(std::uint32_t* param_1)
{
    void* pvVar1 = std::malloc(0xC);

    const std::uintptr_t node_plus_8 =
        static_cast<std::uintptr_t>(reinterpret_cast<std::uintptr_t>(pvVar1)) + 8;

    if (node_plus_8 != 0) {
        *reinterpret_cast<std::uint32_t*>(node_plus_8) = *param_1;
    }
}
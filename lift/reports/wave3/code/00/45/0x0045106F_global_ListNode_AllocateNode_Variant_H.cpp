#include <cstdlib>

std::uint32_t* __fastcall ListNode_AllocateNode_Variant_H(std::uint32_t* param_1)
{
    *param_1 = 0;
    void* const pvVar1 = std::malloc(0x18);
    *param_1 = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(pvVar1));
    return param_1;
}
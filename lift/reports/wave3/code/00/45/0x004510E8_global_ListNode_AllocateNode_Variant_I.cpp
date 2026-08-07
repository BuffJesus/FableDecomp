#include <cstdlib>
#include <cstdint>

std::uint32_t* __fastcall ListNode_AllocateNode_Variant_I(std::uint32_t* param_1)
{
    *param_1 = 0;
    *param_1 = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(std::malloc(0x18)));
    return param_1;
}
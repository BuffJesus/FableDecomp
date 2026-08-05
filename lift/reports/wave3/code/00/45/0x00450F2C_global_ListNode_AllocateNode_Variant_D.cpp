#include <cstdlib>

void** __fastcall ListNode_AllocateNode_Variant_D(void** out_node)
{
    *out_node = nullptr;
    *out_node = std::malloc(0x18);
    return out_node;
}
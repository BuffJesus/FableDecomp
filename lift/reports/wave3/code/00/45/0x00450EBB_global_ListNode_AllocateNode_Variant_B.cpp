#include <cstdlib>

void** __fastcall ListNode_AllocateNode_Variant_B(void** outNode)
{
    *outNode = nullptr;
    *outNode = std::malloc(0x18);
    return outNode;
}
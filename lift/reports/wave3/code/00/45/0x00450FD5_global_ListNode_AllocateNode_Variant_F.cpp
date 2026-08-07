#include <cstdlib>

void** __fastcall ListNode_AllocateNode_Variant_F(void** outNode)
{
    *outNode = nullptr;
    *outNode = std::malloc(0x24);
    return outNode;
}
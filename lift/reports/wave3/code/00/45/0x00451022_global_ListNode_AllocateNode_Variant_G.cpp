void** __fastcall ListNode_AllocateNode_Variant_G(void** outNode)
{
    *outNode = nullptr;
    *outNode = ::malloc(0x18);
    return outNode;
}
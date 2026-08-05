void** __fastcall ListNode_AllocateNode_Variant_O(void** outNode)
{
    *outNode = nullptr;
    void* node = std::malloc(0x18);
    *outNode = node;
    return outNode;
}
void** __fastcall ListNode_AllocateNode_Variant_N(void** outNode)
{
    *outNode = nullptr;
    void* const allocated = std::malloc(0x18);
    *outNode = allocated;
    return outNode;
}
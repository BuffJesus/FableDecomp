void** __fastcall ListNode_AllocateNode_Variant_J(void** out_node)
{
    *out_node = nullptr;

    void* const node = std::malloc(0x18);
    *out_node = node;

    return out_node;
}
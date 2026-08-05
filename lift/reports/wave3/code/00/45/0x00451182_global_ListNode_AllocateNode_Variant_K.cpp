void** __fastcall ListNode_AllocateNode_Variant_K(void** param_1)
{
    *param_1 = nullptr;
    void* const pvVar1 = std::malloc(0x18);
    *param_1 = pvVar1;
    return param_1;
}
struct ListNode_AllocateNode_Variant_E_Out {
    void* value;
};

static_assert(offsetof(ListNode_AllocateNode_Variant_E_Out, value) == 0x0);
static_assert(sizeof(ListNode_AllocateNode_Variant_E_Out) == 0x4);

ListNode_AllocateNode_Variant_E_Out* __fastcall ListNode_AllocateNode_Variant_E(
    ListNode_AllocateNode_Variant_E_Out* param_1)
{
    param_1->value = nullptr;
    void* const pvVar1 = std::malloc(0x18);
    param_1->value = pvVar1;
    return param_1;
}
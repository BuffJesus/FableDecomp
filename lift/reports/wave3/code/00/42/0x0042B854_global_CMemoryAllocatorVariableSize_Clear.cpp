using ulong = unsigned long;

struct CMemoryAllocatorVariableSize_Clear_IndirectOverlay {
    ulong value0;
    ulong value4;
    ulong value8;
};
static_assert(offsetof(CMemoryAllocatorVariableSize_Clear_IndirectOverlay, value8) == 8);

// Shim for the observed `BinarySearchTree_EraseNode(param_2)` call shape.
// The current allocator/context is carried in hidden ECX, and the post-call ECX
// value is exposed separately through `extraout_ECX`.
static void __thiscall BinarySearchTree_EraseNode_Impl(
    CMemoryAllocatorVariableSize* currentAllocator,
    ulong node,
    CMemoryAllocatorVariableSize*& extraout_ECX);

void __thiscall CMemoryAllocatorVariableSize_Clear(
    CMemoryAllocatorVariableSize* param_1,
    ulong param_2,
    ulong param_3)
{
    ulong uVar1;
    CMemoryAllocatorVariableSize* extraout_ECX;

#define BinarySearchTree_EraseNode(node_) \
    BinarySearchTree_EraseNode_Impl(param_1, (node_), extraout_ECX)

    if ((param_2 ==
         reinterpret_cast<const CMemoryAllocatorVariableSize_Clear_IndirectOverlay*>(
             *reinterpret_cast<const ulong*>(param_1))->value8) &&
        (param_3 == *reinterpret_cast<const ulong*>(param_1))) {
        CTCCarryable::OnKill(reinterpret_cast<CTCCarryable*>(param_1));
    } else {
        while (param_2 != param_3) {
            uVar1 = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(param_1);
            BinarySearchTree_EraseNode(param_2);
            param_1 = extraout_ECX;
            param_2 = uVar1;
        }
    }

#undef BinarySearchTree_EraseNode
}
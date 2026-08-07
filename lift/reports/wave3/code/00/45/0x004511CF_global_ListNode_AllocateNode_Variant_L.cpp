#include <cstddef>
#include <cstdlib>

struct ListNodeAllocateNodeVariantLOut
{
    void* value;
};

static_assert(offsetof(ListNodeAllocateNodeVariantLOut, value) == 0x0);

ListNodeAllocateNodeVariantLOut* __fastcall ListNode_AllocateNode_Variant_L(
    ListNodeAllocateNodeVariantLOut* out_node)
{
    out_node->value = nullptr;
    void* const allocated = std::malloc(0x18);
    out_node->value = allocated;
    return out_node;
}
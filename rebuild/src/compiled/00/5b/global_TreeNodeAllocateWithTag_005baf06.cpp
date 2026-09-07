#pragma optimize("s",on)
#include <cstdlib>
struct TreeNodeBytePayload
{
    unsigned long value;
    unsigned char tag;
};
void* __stdcall TreeNode_AllocateWithTag(TreeNodeBytePayload* input)
{
    void* allocation = std::malloc(0x18);
    TreeNodeBytePayload* payload =
        (TreeNodeBytePayload*)((unsigned char*)allocation + 0x10);
    if (payload != 0)
    {
        payload->value = input->value;
        payload->tag = input->tag;
    }
    return allocation;
}
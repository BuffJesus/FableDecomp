#pragma optimize("s",on)
#include <cstdlib>
struct TreeNodePayload
{
    unsigned long first;
    unsigned long second;
};
void* __stdcall BinaryTree_AllocateNode(TreeNodePayload* input)
{
    void* allocation = std::malloc(0x18);
    TreeNodePayload* payload =
        (TreeNodePayload*)((unsigned char*)allocation + 0x10);
    if (payload != 0)
        *payload = *input;
    return allocation;
}
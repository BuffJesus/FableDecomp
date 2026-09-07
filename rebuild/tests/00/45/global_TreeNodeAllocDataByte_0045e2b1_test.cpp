#include <cstdio>
#include <cstdlib>
struct Payload { unsigned long value; unsigned char tag; };
static void* Run(const Payload& input)
{
    unsigned char* allocation = (unsigned char*)std::malloc(0x18);
    if (!allocation) return 0;
    Payload* payload = (Payload*)(allocation + 0x10);
    payload->value = input.value;
    payload->tag = input.tag;
    return allocation;
}
int main()
{
    Payload input = {0x89abcdefUL, 0x5a};
    unsigned char* allocation = (unsigned char*)Run(input);
    if (!allocation) return 1;
    Payload* payload = (Payload*)(allocation + 0x10);
    if (payload->value != input.value || payload->tag != input.tag) return 2;
    std::free(allocation);
    std::puts("TREE_NODE_ALLOC_BYTE_E2B1_PASS");
    return 0;
}
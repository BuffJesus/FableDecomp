#include <stdio.h>

struct Node { unsigned char unused[0x18]; unsigned long value; };

static unsigned long Read(void* node, void* header)
{
    if (node == header) return 0xffffffffUL;
    return *(unsigned long*)((unsigned char*)node + 0x18);
}

int main()
{
    Node node;
    Node header;
    node.value = 0x12345678UL;
    if (Read(&header, &header) != 0xffffffffUL) return 1;
    if (Read(&node, &header) != 0x12345678UL) return 2;
    puts("FID_MAX_SIZE_PASS");
    return 0;
}
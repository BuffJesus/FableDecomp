#include "fable_engine_primitive_list.h"
#include <stdio.h>
#include <string.h>

// Normalize links to byte offsets so native and emulated addresses compare.
static long Link(void* address, CEngineInternalPrimitiveBase* nodes,
                 CEngineInternalPrimitiveBase** head)
{
    if (!address) return -1;
    if (address == head) return -2;
    return (char*)address - (char*)nodes;
}

int main(int argc, char** argv)
{
    if (argc != 2) return 2;
    FILE* input = fopen(argv[1], "r");
    if (!input) return 3;
    unsigned count;
    while (fscanf(input, "%u", &count) == 1)
    {
        if (count > 64) return 4;
        CEngineInternalPrimitiveBase nodes[64];
        memset(nodes, 0xA5, sizeof(nodes));
        CEngineInternalPrimitiveBase* head = 0;
        for (unsigned i = 0; i < count; ++i)
        {
            if (fscanf(input, "%lu", &nodes[i].RenderLayerMask) != 1) return 5;
            FablePrimitiveAddToList(&nodes[i], 0, &head);
        }
        printf("%ld", Link(head, nodes, &head));
        for (unsigned j = 0; j < count; ++j)
        {
            printf(" %ld %ld %ld %ld",
                Link(nodes[j].NextPrimitive, nodes, &head),
                Link(nodes[j].RefPrimitive, nodes, &head),
                Link(nodes[j].NextLayerMask, nodes, &head),
                Link(nodes[j].RefLayerMask, nodes, &head));
        }
        puts("");
    }
    fclose(input);
    return 0;
}

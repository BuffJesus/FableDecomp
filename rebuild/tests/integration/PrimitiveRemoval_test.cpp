#include "fable_engine_primitive_list.h"
#include "engine/_quarantine/CEngineSceneGridCell.h"
#include <stdio.h>
#include <string.h>

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
    unsigned count, removals, flags, withGrid;
    while (fscanf(input, "%u %u %u %u", &count, &removals, &flags, &withGrid) == 4)
    {
        if (count > 64) return 4;
        CEngineInternalPrimitiveBase nodes[64];
        CEngineSceneGridCell grid;
        memset(nodes, 0, sizeof(nodes));
        memset(&grid, 0, sizeof(grid));
        grid.LocalCount = 100;
        CEngineInternalPrimitiveBase* head = 0;
        for (unsigned i = 0; i < count; ++i)
        {
            if (fscanf(input, "%lu", &nodes[i].RenderLayerMask) != 1) return 5;
            nodes[i].PostRenderEnable[0] = static_cast<unsigned char>(flags);
            nodes[i].GridCell = withGrid ? &grid : 0;
            FablePrimitiveAddToList(&nodes[i], 0, &head);
        }
        for (unsigned r = 0; r < removals; ++r)
        {
            unsigned index;
            if (fscanf(input, "%u", &index) != 1 || index >= count) return 6;
            FablePrimitiveRemoveFromList(&nodes[index], 0);
            printf("%ld %ld", Link(head, nodes, &head), grid.LocalCount);
            for (unsigned j = 0; j < count; ++j)
                printf(" %ld %ld %ld %ld %u",
                    Link(nodes[j].NextPrimitive, nodes, &head),
                    Link(nodes[j].RefPrimitive, nodes, &head),
                    Link(nodes[j].NextLayerMask, nodes, &head),
                    Link(nodes[j].RefLayerMask, nodes, &head),
                    nodes[j].GridCell ? 1 : 0);
            puts("");
        }
    }
    fclose(input);
    return 0;
}

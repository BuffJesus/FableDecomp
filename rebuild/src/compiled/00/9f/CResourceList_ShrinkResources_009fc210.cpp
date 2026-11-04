#include "fable_resource_list.h"
void CResourceList::ShrinkResources(unsigned long size)
{
    unsigned long attempts = 0;
    while (AllocatedMemory + size > (unsigned long)MaximumMemory && attempts++ < ResourceCount)
        if (!UnloadNextResource(0)) break;
}
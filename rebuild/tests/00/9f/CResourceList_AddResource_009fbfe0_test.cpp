#include <stdio.h>
#include "engine/CResource.h"
#include "engine/CResourceList.h"

struct CResourceListRetail {
    void* __vftable;
    CResource Head;
    unsigned long ResourceCount;
    long AllocatedMemory;
    long MaximumMemory;
    unsigned long CurrentFrame;
    unsigned long DebugStatsFrame;
    unsigned long UnloadDelay;
    unsigned long UnloadedThisFrame;
    void AddResource(CResource& resource);
};

int main()
{
    CResourceListRetail list = {};
    CResource resource = {};
    list.Head.PrevResource = &list.Head;
    list.Head.NextResource = &list.Head;
    list.CurrentFrame = 73;
    list.AllocatedMemory = 1000;
    resource.ResourceSize = 256;

    list.AddResource(resource);

    if (resource.ResourceList != reinterpret_cast<CResourceList*>(&list) ||
        resource.LastUsedFrame != 73)
        return 1;
    if (resource.PrevResource != &list.Head ||
        resource.NextResource != &list.Head ||
        list.Head.PrevResource != &resource ||
        list.Head.NextResource != &resource)
        return 2;
    if (list.ResourceCount != 1 || list.AllocatedMemory != 1256)
        return 3;

    printf("ADD_RESOURCE PASS\n");
    return 0;
}

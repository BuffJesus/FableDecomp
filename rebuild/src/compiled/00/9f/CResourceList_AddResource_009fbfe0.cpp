#pragma optimize("s",on)
#include "engine/CResource.h"
#include "engine/CResourceList.h"

// The generated donor header flattens Head to bytes. Retail accesses it as the
// original embedded CResource sentinel, so expose that composite type here.
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

void CResourceListRetail::AddResource(CResource& resource)
{
    resource.ResourceList = reinterpret_cast<CResourceList*>(this);
    resource.LastUsedFrame = CurrentFrame;

    resource.PrevResource = Head.PrevResource;
    resource.NextResource = &Head;
    resource.PrevResource->NextResource = &resource;
    resource.NextResource->PrevResource = &resource;

    ++ResourceCount;
    AllocatedMemory += resource.ResourceSize;
}

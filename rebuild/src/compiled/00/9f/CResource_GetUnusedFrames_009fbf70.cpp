#include "fable_resource_list.h"
unsigned long CResource::GetUnusedFrames()const{if(ResourceList)return ResourceList->CurrentFrame-LastUsedFrame;return ~0UL;}

// Retail 009FC0B0: debug memory of the resources used within `frames`
// frames, newest first.
unsigned long CResourceList::GetDebugMemoryInUse(unsigned long frames) const
{
    unsigned long total = 0;
    for (const CResource* resource = Head.PrevResource; resource != &Head; resource = resource->PrevResource)
    {
        if (resource->GetUnusedFrames() > frames)
            break;
        total += resource->GetDebugMemoryInUse();
    }
    return total;
}

// Retail 009FC100.
void CResourceList::ResetDebugStats()
{
    for (CResource* resource = Head.NextResource; resource != &Head; resource = resource->NextResource)
        resource->ResetDebugMemoryInUse();
    DebugStatsFrame = CurrentFrame;
}

// Retail 009FC140.
unsigned long CResourceList::GetFramesSinceResetDebugStats() const
{
    return CurrentFrame - DebugStatsFrame;
}

// Retail 009FC360: walks the list (its checks are debug-only).
void CResourceList::Validate()
{
    for (CResource* resource = Head.NextResource; resource != &Head; resource = resource->NextResource)
    {
    }
}

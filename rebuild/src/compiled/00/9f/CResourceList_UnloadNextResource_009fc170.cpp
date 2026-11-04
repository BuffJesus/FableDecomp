// Retail 009fc170: behavior-tested near match (157 versus 149 bytes).
#include "fable_resource_list.h"
bool CResourceList::UnloadNextResource(unsigned long size)
{
    CResource* resource = Head.NextResource;
    if (resource == &Head || UnloadedThisFrame >= ResourceCount)
        return false;
    bool unloaded = false;
    if (CurrentFrame - resource->LastUsedFrame >= UnloadDelay &&
        resource->ResourceSize >= size)
    {
        resource->AddRef();
        unloaded = resource->UnloadResource();
        if (--resource->IVRefCount == 0)
        {
            resource->DeleteCIVCountedPointeeBase();
            return true;
        }
    }
    if (resource->ResourceList == this)
    {
        resource->ListRemove();
        resource->PrevResource = Head.PrevResource;
        resource->NextResource = &Head;
        resource->PrevResource->NextResource = resource;
        resource->NextResource->PrevResource = resource;
        if (unloaded)
            resource->LastUsedFrame = CurrentFrame;
        ++UnloadedThisFrame;
    }
    return true;
}

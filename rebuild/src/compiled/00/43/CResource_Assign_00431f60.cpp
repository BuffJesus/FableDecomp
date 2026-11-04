#include "fable_resource_list.h"

CResource& CResource::operator=(const CResource& other)
{
    IVRefCount = other.IVRefCount;
    ResourceList = other.ResourceList;
    PrevResource = other.PrevResource;
    NextResource = other.NextResource;
    ResourceSize = other.ResourceSize;
    LastUsedFrame = other.LastUsedFrame;
    return *this;
}

#pragma optimize("s",on)
#include "engine/CResource.h"

void __fastcall CResource_ListRemove(CResource* resource)
{
    if (resource->NextResource == 0)
        return;

    const bool neighboursReferenceResource =
        resource->PrevResource->NextResource == resource &&
        resource->NextResource->PrevResource == resource;

    if (neighboursReferenceResource)
    {
        resource->PrevResource->NextResource = resource->NextResource;
        resource->NextResource->PrevResource = resource->PrevResource;
    }

    // Preserve retail's explicit pointer stores. VC7.1 otherwise changes these
    // assignments into shorter `and` instructions under size optimisation.
    *reinterpret_cast<CResource* volatile*>(&resource->PrevResource) = 0;
    *reinterpret_cast<CResource* volatile*>(&resource->NextResource) = 0;
}

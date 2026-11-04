// CResourceBank::RemoveResource, retail 0x009FC3C0 (ResourceList.cpp): a tail
// call into the resource list. Own file so fixtures linking AddResource
// need no CResourceList::RemoveResource.
#include "fable_game_text_bank.h"

void CResourceBank::RemoveResource(CResource& resource)
{
    ResourceList.RemoveResource(resource);
}

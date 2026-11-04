#pragma optimize("s", on)
#include "fable_resource_list.h"
void CResourceList::AddResource(CResource& resource) {
 resource.ResourceList = this; resource.LastUsedFrame = CurrentFrame;
 resource.PrevResource = Head.PrevResource; resource.NextResource = &Head;
 resource.PrevResource->NextResource = &resource; resource.NextResource->PrevResource = &resource;
 ++ResourceCount; AllocatedMemory += resource.ResourceSize;
}

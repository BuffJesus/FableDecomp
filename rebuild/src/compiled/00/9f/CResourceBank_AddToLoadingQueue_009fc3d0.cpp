#include "fable_game_text_bank.h"
inline void CResourceList::AddResource(CResource& resource) {
 resource.ResourceList = this; resource.LastUsedFrame = CurrentFrame;
 resource.PrevResource = Head.PrevResource; resource.NextResource = &Head;
 resource.PrevResource->NextResource = &resource; resource.NextResource->PrevResource = &resource;
 ++ResourceCount; AllocatedMemory += resource.ResourceSize;
}
void CResourceBank::AddToLoadingQueue(CResource& resource) {LoadingQueue.AddResource(resource);}

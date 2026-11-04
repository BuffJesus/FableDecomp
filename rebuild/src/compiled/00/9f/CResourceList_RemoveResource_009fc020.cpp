#include "fable_game_text_bank.h"
extern double FABLE_FASTCALL GFGetTime();

inline void CResourceList::RemoveResource(CResource& resource)
{
    resource.ResourceList = 0;
    if (resource.NextResource)
    {
        if (resource.PrevResource->NextResource == &resource &&
            resource.NextResource->PrevResource == &resource)
        {
            resource.PrevResource->NextResource = resource.NextResource;
            resource.NextResource->PrevResource = resource.PrevResource;
        }
        resource.PrevResource = 0;
        resource.NextResource = 0;
    }
    --ResourceCount;
    AllocatedMemory -= resource.ResourceSize;
}

void CResourceList::ProcessLoadingQueue(bool bounded,double deadline){
 CResource* resource=Head.NextResource;
 while(resource!=&Head){
  if(!resource)return;
  if(bounded && GFGetTime()>deadline)return;
  CResource* next=resource->NextResource;
  if(CResourceBank::IsResourceAbortingEnabled() && resource->CanAbortResourceLoad(CurrentFrame-resource->LastUsedFrame)){
   RemoveResource(*resource);resource->OnResourceLoadAborted();next=Head.NextResource;
  }else if(resource->IsResourceLoaded()){
   RemoveResource(*resource);resource->OnResourceLoaded();next=Head.NextResource;
  }
  resource=next;
 }
}

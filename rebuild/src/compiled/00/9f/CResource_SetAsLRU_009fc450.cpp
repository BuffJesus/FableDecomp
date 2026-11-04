#include "fable_resource_list.h"
void CResource::SetAsLRU() {
    if(ResourceList) {
        if(NextResource) {
            if(PrevResource->NextResource==this && NextResource->PrevResource==this) {
                PrevResource->NextResource=NextResource;
                NextResource->PrevResource=PrevResource;
            }
            PrevResource=0;
            NextResource=0;
        }
        PrevResource=&ResourceList->Head;
        NextResource=ResourceList->Head.NextResource;
        PrevResource->NextResource=this;
        NextResource->PrevResource=this;
    }
}

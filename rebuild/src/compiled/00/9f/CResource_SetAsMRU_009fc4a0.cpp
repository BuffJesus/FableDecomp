#include "fable_resource_list.h"
void CResource::SetAsMRU() {
    if(ResourceList) {
        if(NextResource) {
            if(PrevResource->NextResource==this && NextResource->PrevResource==this) {
                PrevResource->NextResource=NextResource;
                NextResource->PrevResource=PrevResource;
            }
            PrevResource=0;
            NextResource=0;
        }
        NextResource=&ResourceList->Head;
        PrevResource=ResourceList->Head.PrevResource;
        PrevResource->NextResource=this;
        NextResource->PrevResource=this;
    }
}

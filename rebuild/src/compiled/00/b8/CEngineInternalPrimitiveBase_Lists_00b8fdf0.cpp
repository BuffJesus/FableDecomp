#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineSceneGridCell.h"

void CEngineInternalPrimitiveBase::RemoveFromList()
{
    if(!(PostRenderEnable[0]&2) && GridCell) {
        --GridCell->LocalCount;
        GridCell=0;
    }
    if(RefPrimitive)*RefPrimitive=NextPrimitive;
    if(NextPrimitive)NextPrimitive->RefPrimitive=RefPrimitive;
    if(RefLayerMask) {
        *RefLayerMask=NextPrimitive;
        if(NextPrimitive) {
            NextPrimitive->RefLayerMask=RefLayerMask;
            if(NextLayerMask && NextLayerMask!=NextPrimitive) {
                NextPrimitive->NextLayerMask=NextLayerMask;
                NextLayerMask->RefLayerMask=&NextPrimitive->NextLayerMask;
            }
        }
    }
    NextPrimitive=0;
    RefPrimitive=0;
    NextLayerMask=0;
    RefLayerMask=0;
}

void CEngineInternalPrimitiveBase::AddToList(CEngineInternalPrimitiveBase** head)
{
    unsigned long mask=RenderLayerMask;
    CEngineInternalPrimitiveBase* first=*head;
    CEngineInternalPrimitiveBase* group=first;
    while(group && group->RenderLayerMask!=mask)group=group->NextLayerMask;
    if(!group) {
        NextPrimitive=first;
        NextLayerMask=first;
        if(first) {
            first->RefPrimitive=&NextPrimitive;
            first->RefLayerMask=&NextLayerMask;
        }
        RefPrimitive=head;
        RefLayerMask=head;
        *head=this;
    } else {
        NextPrimitive=group->NextPrimitive;
        RefPrimitive=&group->NextPrimitive;
        group->NextPrimitive=this;
        if(NextPrimitive)NextPrimitive->RefPrimitive=&NextPrimitive;
        NextLayerMask=0;
        RefLayerMask=0;
    }
}

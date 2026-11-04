// Native boot progress-display ownership provider, validated against retail and shared lifetime composition.
#include "fable_boot.h"
void FABLE_FASTCALL FableDestroyProgressDisplay_00403600(void* object);
CCountedProgressDisplay::CCountedProgressDisplay(CProgressDisplay* value)
{
    object=value;
    if(value){
        FableReferenceCount* block=static_cast<FableReferenceCount*>(FableOperatorNew(sizeof(FableReferenceCount)));
        FableReferenceCount* result;
        if(block){block->owners=1;block->destroy=FableDestroyProgressDisplay_00403600;block->object=object;result=block;}
        else result=0;
        reference=result;
    }else reference=0;
}

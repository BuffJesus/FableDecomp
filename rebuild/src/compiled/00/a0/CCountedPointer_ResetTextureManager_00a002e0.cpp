#include "fable_ui_bank_runtime.h"
void __fastcall FableUiResetTextureManager(FableUiBankReference* reference,void*,void* object)
{
    if(reference->Info)
    {
        FableReferenceCount* info=static_cast<FableReferenceCount*>(reference->Info);
        --info->owners;
        if(static_cast<FableReferenceCount*>(reference->Info)->owners==0)
        {
            info=static_cast<FableReferenceCount*>(reference->Info);
            info->destroy(info->object);
            FableUiDeleteReference(static_cast<FableReferenceCount*>(reference->Info));
        }
    }
    reference->Info=0; reference->Data=object;
    if(object)
    {
        FableReferenceCount* info=static_cast<FableReferenceCount*>(FableUiAllocateGraphicsBank(12));
        if(info) { info->owners=1; info->destroy=FableUiDeleteTextureManager; info->object=reference->Data; }
        reference->Info=info;
    }
}

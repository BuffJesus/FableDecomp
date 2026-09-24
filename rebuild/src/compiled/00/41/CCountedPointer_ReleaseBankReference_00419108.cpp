#include "fable_ui_bank_ownership.h"

void __fastcall FableUiReleaseBankReference(FableUiBankReference* reference,void*)
{
    if (reference->Info)
    {
        --static_cast<FableReferenceCount*>(reference->Info)->owners;
        if (static_cast<FableReferenceCount*>(reference->Info)->owners==0)
        {
            FableReferenceCount* info=static_cast<FableReferenceCount*>(reference->Info);
            info->destroy(info->object);
            // Retail reloads Info after the callback, before freeing the record.
            FableUiDeleteReference(static_cast<FableReferenceCount*>(reference->Info));
        }
    }
    reference->Data=0;
    reference->Info=0;
}

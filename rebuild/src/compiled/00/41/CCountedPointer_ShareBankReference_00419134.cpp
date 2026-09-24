#include "fable_ui_bank_ownership.h"

void __fastcall FableUiShareBankReference(FableUiBankReference* reference,void*,void* data,void* info)
{
    if (reference->Info!=info)
    {
        FableUiReleaseBankReference(reference,0);
        reference->Data=data;
        reference->Info=info;
        if (info) ++static_cast<FableReferenceCount*>(info)->owners;
    }
}

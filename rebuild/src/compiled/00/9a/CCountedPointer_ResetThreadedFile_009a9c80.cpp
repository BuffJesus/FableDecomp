#include "fable_ui_bank_open.h"
void __fastcall FableUiResetThreadedFile(FableUiBankReference* reference,void*,CThreadedFile* file)
{
    FableUiReleaseBankReference(reference,0);
    reference->Data=file;
    if(file)
    {
        FableReferenceCount* info=static_cast<FableReferenceCount*>(FableUiAllocateGraphicsBank(12));
        if(info) { info->owners=1; info->destroy=FableUiDeleteThreadedFile; info->object=reference->Data; }
        reference->Info=info;
    }
}

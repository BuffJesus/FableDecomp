#include "fable_ui_bank_open.h"
FableUiBankReference* __fastcall FableUiAssignDiskFile(FableUiBankReference* reference,void*,const FableUiBankReference* source)
{
    void* data=source->Data; void* info=source->Info;
    FableUiShareBankReference(reference,0,data,info);
    return reference;
}

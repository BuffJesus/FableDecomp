#include "fable_ui_bank_aliases.h"
void __fastcall FableUiResizeBankAliases(FableUiBankAliases* self,void*,unsigned count)
{
    FableUiClearBankAliases(self,0);
    unsigned* allocation=static_cast<unsigned*>(FableUiAllocateStringBuffer(count*4+4));
    FableUiStringValue* data=0;
    if(allocation)
    {
        *allocation=count; data=reinterpret_cast<FableUiStringValue*>(allocation+1);
        if(static_cast<long>(count-1)>=0)
            for(unsigned i=0;i<count;++i) FableUiConstructEmptyString(data+i,0);
    }
    self->Data=data; self->Count=static_cast<signed char>(count);
}

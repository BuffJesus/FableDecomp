#include "fable_ui_bank_aliases.h"
void __fastcall FableUiClearBankAliases(FableUiBankAliases* self,void*)
{
    if(self->Data)
    {
        unsigned* allocation=reinterpret_cast<unsigned*>(self->Data)-1;
        unsigned count=*allocation;
        FableUiStringValue* end=self->Data+count;
        if(static_cast<long>(count-1)>=0)
            for(unsigned i=0;i<count;++i) FableUiDestroyBankName(--end,0);
        FableUiFreeStringBuffer(allocation);
    }
    self->Data=0; self->Count=0;
}

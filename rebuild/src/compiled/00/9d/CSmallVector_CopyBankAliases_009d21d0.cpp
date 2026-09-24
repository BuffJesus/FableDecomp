#include "fable_ui_bank_aliases.h"
void __fastcall FableUiCopyBankAliases(FableUiBankAliases* self,void*,const FableUiBankAliases* source)
{
    FableUiResizeBankAliases(self,0,source->Count);
    // Retail deliberately has no self-assignment guard; callers supply one.
    for(unsigned i=0;i<static_cast<unsigned>(source->Count);++i)
        FableUiAssignString(self->Data+i,0,source->Data+i);
}

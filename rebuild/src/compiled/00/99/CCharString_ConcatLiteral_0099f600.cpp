#include "fable_ui_strings.h"
FableUiStringValue* __fastcall FableUiConcatStringLiteral(FableUiStringValue* result,const FableUiStringValue* left,const char* right)
{
    FableUiStringValue temporary;
    FableUiCopyString(&temporary,0,left);
    FableUiAppendStringLiteral(&temporary,0,right);
    FableUiCopyString(result,0,&temporary);
    FableUiDestroyBankName(&temporary,0);
    return result;
}

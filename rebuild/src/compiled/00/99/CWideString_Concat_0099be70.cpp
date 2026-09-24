#include "fable_ui_wide_strings.h"
FableUiWideStringValue* __fastcall FableUiConcatWideStrings(FableUiWideStringValue* result,const FableUiWideStringValue* left,const FableUiWideStringValue* right)
{
    FableUiWideStringValue temporary;
    FableUiCopyWideString(&temporary,0,left);
    FableUiAppendWideString(&temporary,0,right);
    FableUiCopyWideString(result,0,&temporary);
    FableUiDestroyWideString(&temporary,0);
    return result;
}

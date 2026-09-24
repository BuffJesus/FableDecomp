#include "fable_ui_wide_strings.h"
FableUiWideStringValue* __fastcall FableUiCopyWideString(FableUiWideStringValue* value,void*,const FableUiWideStringValue* other)
{
    value->Storage=0; ++g_CWideStringInstanceCount_013BCA20;
    return FableUiAssignWideString(value,0,other);
}

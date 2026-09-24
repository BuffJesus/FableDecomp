#include "fable_ui_wide_strings.h"
FableUiWideStringValue* __fastcall FableUiConstructWideText(FableUiWideStringValue* value,void*,const wchar_t* text)
{
    value->Storage=0; ++g_CWideStringInstanceCount_013BCA20;
    if(text && *text) value->Storage=FableUiAllocateWideData(value,0,text,-1);
    return value;
}

#include "fable_ui_wide_strings.h"
void __fastcall FableUiMakeWideStringUnique(FableUiWideStringValue* value,void*)
{
    if(value->Storage && value->Storage->owners>1)
    {
        const wchar_t* text=value->Storage->text;
        CWideStringData* data=FableUiAllocateWideData(value,0,text,-1);
        FableUiUnassignWideString(value,0);
        value->Storage=data;
    }
}

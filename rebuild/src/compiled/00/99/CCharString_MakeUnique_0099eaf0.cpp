#include "fable_ui_strings.h"
void __fastcall FableUiMakeStringUnique(FableUiStringValue* value,void*)
{
    if(value->Storage && value->Storage->owners>1)
    {
        const char* text=value->Storage->text;
        CCharStringData* data=FableUiAllocateStringData(value,0,text,-1);
        FableUiUnassignString(value,0);
        value->Storage=data;
    }
}

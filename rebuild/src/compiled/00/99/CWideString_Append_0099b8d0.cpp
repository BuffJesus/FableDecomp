#include "fable_ui_wide_strings.h"
FableUiWideStringValue* __fastcall FableUiAppendWideString(FableUiWideStringValue* value,void*,const FableUiWideStringValue* other)
{
    if(value->Storage)
    {
        FableUiMakeWideStringUnique(value,0);
        const wchar_t* text=other->Storage ? other->Storage->text : g_FableEmptyWideString_0129A8E0;
        FableUiAppendWideRange(value->Storage,0,text,text+FableUiWideLength(text),0);
    }
    else if(value!=other && other->Storage)
    { value->Storage=other->Storage; ++value->Storage->owners; }
    return value;
}

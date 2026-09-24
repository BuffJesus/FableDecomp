#include "fable_ui_strings.h"
#include <string.h>
FableUiStringValue* __fastcall FableUiAppendStringLiteral(FableUiStringValue* value,void*,const char* text)
{
    if(value->Storage)
    {
        FableUiMakeStringUnique(value,0);
        FableUiAppendStringBytes(value->Storage,0,text,strlen(text));
    }
    else if(text && *text) value->Storage=FableUiAllocateStringData(value,0,text,-1);
    return value;
}

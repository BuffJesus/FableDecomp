#include "fable_ui_strings.h"
FableUiStringValue* __fastcall FableUiAssignString(FableUiStringValue* value,void*,const FableUiStringValue* source)
{
    if(value!=source)
    {
        if(value->Storage) FableUiUnassignString(value,0);
        if(source->Storage) { value->Storage=source->Storage; ++value->Storage->owners; }
    }
    return value;
}

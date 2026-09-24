#include "fable_ui_wide_strings.h"
FableUiWideStringValue* __fastcall FableUiAssignWideString(FableUiWideStringValue* value,void*,const FableUiWideStringValue* other)
{
    if(value!=other && value->Storage!=other->Storage)
    {
        if(value->Storage) FableUiUnassignWideString(value,0);
        if(other->Storage) { value->Storage=other->Storage; ++value->Storage->owners; }
    }
    return value;
}

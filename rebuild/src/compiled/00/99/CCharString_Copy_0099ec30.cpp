#include "fable_ui_strings.h"
FableUiStringValue* __fastcall FableUiCopyString(FableUiStringValue* value,void*,const FableUiStringValue* source)
{
    value->Storage=0; ++g_CCharStringInstanceCount_013BD800;
    if(value!=source)
    {
        if(value->Storage) FableUiUnassignString(value,0);
        if(source->Storage) { value->Storage=source->Storage; ++value->Storage->owners; }
    }
    return value;
}

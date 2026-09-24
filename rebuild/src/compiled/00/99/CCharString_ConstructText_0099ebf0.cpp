#include "fable_ui_strings.h"
FableUiStringValue* __fastcall FableUiConstructBankName(FableUiStringValue* value,void*,const char* text,long length)
{
    value->Storage=0; ++g_CCharStringInstanceCount_013BD800;
    if(value->Storage) FableUiUnassignString(value,0);
    if(text && *text) value->Storage=FableUiAllocateStringData(value,0,text,length);
    return value;
}

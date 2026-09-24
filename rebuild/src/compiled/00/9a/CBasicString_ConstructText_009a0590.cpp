#include "fable_ui_strings.h"
CCharStringData* __fastcall FableUiConstructStringData(CCharStringData* data,void*,const char* source)
{
    data->unknown08&=0x80000000u; data->flags0C|=1;
    data->text=0; data->unknown04=0;
    unsigned length=0; while(source[length]) ++length;
    FableUiAssignStringBytes(data,0,source,length);
    return data;
}

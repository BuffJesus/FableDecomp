#include "fable_ui_strings.h"
#include <string.h>
void __fastcall FableUiAppendStringBytes(CCharStringData* data,void*,const char* text,unsigned length)
{
    FableUiReserveStringBytes(data,0,data->unknown04+length);
    char* target=data->text+data->unknown04;
    memcpy(target,text,length); target[length]=0;
    data->unknown04+=length; data->text[data->unknown04]=0;
    unsigned used=data->unknown04+1u;
    memset(data->text+used,0,((used+3u)&~3u)-used);
}

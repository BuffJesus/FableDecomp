#include "fable_ui_strings.h"
#include <string.h>
void __fastcall FableUiAssignStringBytes(CCharStringData* data,void*,const char* source,unsigned length)
{
    if(data->text) { FableUiFreeStringBuffer(data->text); data->text=0; }
    unsigned capacity=(length+4u)&~3u;
    data->text=static_cast<char*>(FableUiAllocateStringBuffer(capacity));
    data->unknown08=(data->unknown08&0x80000000u)|(capacity&0x7fffffffu);
    data->unknown04=length;
    memset(data->text+length,0,capacity-length);
    memcpy(data->text,source,length);
}

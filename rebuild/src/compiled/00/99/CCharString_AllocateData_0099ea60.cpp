#include "fable_ui_strings.h"
CCharStringData* __fastcall FableUiAllocateStringData(FableUiStringValue*,void*,const char* text,long length)
{
    CCharStringData* data=static_cast<CCharStringData*>(FableUiAllocateStringRecord(17));
    if(!data) return 0;
    if(length==-1) FableUiConstructStringData(data,0,text);
    else
    {
        data->unknown08&=0x80000000u; data->flags0C|=1;
        data->text=0; data->unknown04=0;
        FableUiAssignStringBytes(data,0,text,static_cast<unsigned>(length));
    }
    data->owners=1; return data;
}

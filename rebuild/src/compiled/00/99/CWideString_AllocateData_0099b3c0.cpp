#include "fable_ui_wide_strings.h"
CWideStringData* __fastcall FableUiAllocateWideData(FableUiWideStringValue*,void*,const wchar_t* text,long length)
{
    CWideStringData* data=static_cast<CWideStringData*>(FableUiAllocateStringRecord(16));
    if(data)
    {
        data->text=0; data->unknown04=data->unknown08=0;
        if(length==-1) length=FableUiWideLength(text);
        FableUiConstructWideRange(data,0,text,text+length,0);
        data->owners=1;
    }
    return data;
}

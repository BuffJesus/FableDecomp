#include "fable_ui_wide_strings.h"
#include <string.h>
CWideStringData* __fastcall FableUiAppendWideRange(CWideStringData* data,void*,const wchar_t* first,const wchar_t* last,void*)
{
    if(first==last) return data;
    unsigned bytes=reinterpret_cast<unsigned>(last)-reinterpret_cast<unsigned>(first);
    unsigned added=static_cast<int>(bytes)>>1;
    unsigned length=static_cast<int>(data->unknown04-reinterpret_cast<unsigned>(data->text))>>1;
    if(added>0x7ffffffeu || length>0x7ffffffeu-added) FableUiWideLengthError(data,0);
    unsigned capacity=(static_cast<int>(data->unknown08-reinterpret_cast<unsigned>(data->text))>>1)-1;
    if(length+added>capacity)
    {
        unsigned count=length+(length<added ? added : length)+1;
        wchar_t* buffer=count ? static_cast<wchar_t*>(FableUiAllocateWideBuffer(count*2)) : 0;
        if(data->unknown04!=reinterpret_cast<unsigned>(data->text)) memcpy(buffer,data->text,length*2);
        memcpy(buffer+length,first,bytes);
        wchar_t* end=buffer+length+added; *end=0;
        if(data->text) FableUiFreeWideBuffer(data->text);
        data->text=buffer; data->unknown04=reinterpret_cast<unsigned>(end);
        data->unknown08=reinterpret_cast<unsigned>(buffer+count);
    }
    else
    {
        wchar_t* end=reinterpret_cast<wchar_t*>(data->unknown04);
        if(last!=first+1) memcpy(end+1,first+1,bytes-2);
        end[added]=0; *end=*first; data->unknown04+=bytes;
    }
    return data;
}

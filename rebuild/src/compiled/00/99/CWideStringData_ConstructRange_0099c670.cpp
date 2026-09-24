#include "fable_ui_wide_strings.h"
#include <string.h>
void __fastcall FableUiConstructWideRange(CWideStringData* data,void*,const wchar_t* first,const wchar_t* last,void*)
{
    unsigned bytes=reinterpret_cast<unsigned>(last)-reinterpret_cast<unsigned>(first);
    unsigned count=(static_cast<int>(bytes)>>1)+1;
    if(!count || count>0x7fffffffu) FableUiWideLengthError(data,0);
    wchar_t* buffer=static_cast<wchar_t*>(FableUiAllocateWideBuffer(count*2));
    data->unknown08=reinterpret_cast<unsigned>(buffer)+count*2;
    data->text=buffer; data->unknown04=reinterpret_cast<unsigned>(buffer);
    if(first!=last) memcpy(buffer,first,bytes);
    data->unknown04=reinterpret_cast<unsigned>(buffer)+bytes;
    *reinterpret_cast<wchar_t*>(data->unknown04)=0;
}

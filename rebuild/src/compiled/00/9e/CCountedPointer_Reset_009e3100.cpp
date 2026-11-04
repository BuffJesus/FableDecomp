#include "fable_counted_pointer.h"
#include "fable_font_data_banks.h"
template<> void CCountedPointer<CStreamingFontDataBank>::Reset(CStreamingFontDataBank* data)
{
    Release();
    Info = 0;
    Data = data;
    if (Data) Info = new CCPPointerInfo(&DeleteData, (void*)Data);
}
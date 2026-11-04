#include "fable_font_data_banks.h"
unsigned long CStreamingFontDataBank::AdjustUpdateEntries()
{
    unsigned long count=0;
    for(unsigned long i=0;i<Size;++i)
        if(EntryExists(i) && !EntrySetUpdate(i,true)) ++count;
    return count;
}

#include "fable_font_data_banks.h"
#include "engine/CStreamingFontDataBankEntry.h"

void CStreamingFontDataBank::NotifyEntryRemoved(unsigned long index)
{
    CStreamingFontDataBankEntry* entry = Fonts[index];
    if (entry != 0)
        delete entry;
    Fonts[index] = 0;
}

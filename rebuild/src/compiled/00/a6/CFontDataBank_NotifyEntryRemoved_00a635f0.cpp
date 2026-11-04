#include "fable_font_data_banks.h"
#include "engine/CFontDataBankEntry.h"

void CFontDataBank::NotifyEntryRemoved(unsigned long index)
{
    CFontDataBankEntry* entry = Fonts[index];
    if (entry != 0)
        delete entry;
    Fonts[index] = 0;
}

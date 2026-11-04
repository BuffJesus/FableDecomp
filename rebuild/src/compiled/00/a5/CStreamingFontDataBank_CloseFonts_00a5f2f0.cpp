#include "fable_font_data_banks.h"
#include "engine/CStreamingFontDataBankEntry.h"

void CStreamingFontDataBank::CloseFonts()
{
    for (unsigned long index = 0; index < Fonts.size(); ++index)
    {
        CStreamingFontDataBankEntry* entry = Fonts[index];
        if (entry != 0)
        {
            if (entry->IsLoaded())
            {
                entry->CanUnload();
                entry->Unload();
            }
            delete entry;
        }
    }
    Fonts.clear();
    Filename = "";
}
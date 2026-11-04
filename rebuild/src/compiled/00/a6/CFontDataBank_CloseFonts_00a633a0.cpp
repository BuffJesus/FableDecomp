#include "fable_font_data_banks.h"
#include "engine/CFontDataBankEntry.h"

void CFontDataBank::CloseFonts()
{
    while (Fonts._M_start != Fonts._M_finish)
    {
        CFontDataBankEntry* entry = Fonts._M_finish[-1];
        if (entry != 0)
        {
            if (entry->IsLoaded())
            {
                entry->CanUnload();
                entry->Unload();
            }
            delete entry;
            Fonts._M_finish[-1] = 0;
        }
        --Fonts._M_finish;
    }
    Filename = "";
}
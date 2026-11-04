#include "fable_font_data_banks.h"
CStreamingFontDataBankEntry* CStreamingFontDataBank::GetFontInfo(unsigned long index) const
{
    return Fonts[index];
}

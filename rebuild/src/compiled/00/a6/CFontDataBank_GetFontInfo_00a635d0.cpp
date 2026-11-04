#include "fable_font_data_banks.h"
CFontDataBankEntry* CFontDataBank::GetFontInfo(unsigned long index) const
{
    return Fonts[index];
}

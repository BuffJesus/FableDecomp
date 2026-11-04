#include "engine/CFontDataBank.h"
const CFontDataBankEntry* CFontDataBank::PeekFontInfo(unsigned long index) const
{
    return Fonts[index];
}

#include "engine/CStreamingFontDataBank.h"
const CStreamingFontDataBankEntry* CStreamingFontDataBank::PeekFontInfo(unsigned long index) const
{
    return Fonts[index];
}

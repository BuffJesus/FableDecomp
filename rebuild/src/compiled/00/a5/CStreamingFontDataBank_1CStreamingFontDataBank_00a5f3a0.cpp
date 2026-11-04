#include "fable_font_data_banks.h"

inline CStreamingFontBankStateBlock::~CStreamingFontBankStateBlock() {}

CStreamingFontDataBank::~CStreamingFontDataBank()
{
    CloseFonts();
}

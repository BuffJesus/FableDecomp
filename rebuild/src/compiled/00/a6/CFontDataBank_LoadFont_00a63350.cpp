// lib_font_data_bank.cpp, retail 0x00A63350 (Ego_r
// ?LoadFont@CFontDataBank@@QAEPAVCFontBank@@K@Z): a static font bank over
// this bank's entry.
#include "fable_font_data_banks.h"
#include "engine/CStaticFontBank.h"

CFontBank* CFontDataBank::LoadFont(unsigned long index)
{
    return reinterpret_cast<CFontBank*>(new CStaticFontBank(this, index));
}

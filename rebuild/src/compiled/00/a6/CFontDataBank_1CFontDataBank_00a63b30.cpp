#include "fable_font_data_banks.h"

// Retail inlines the empty state-block destructor into the bank's deleting
// destructor. Keeping this definition visible here preserves that codegen.
inline CFontBankStateBlock::~CFontBankStateBlock() {}

CFontDataBank::~CFontDataBank()
{
    CloseFonts();
}

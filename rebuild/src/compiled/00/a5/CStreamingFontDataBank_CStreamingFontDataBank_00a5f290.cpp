#include "fable_font_data_banks.h"
// The state block destructor is empty; inline here so its deleting destructor (0x00A5EE70), emitted with the
// vtable in this unit, reduces to restoring the CBankStateBlock vtable as in retail.
inline CStreamingFontBankStateBlock::~CStreamingFontBankStateBlock() {}
CStreamingFontDataBank::CStreamingFontDataBank()
{
    Filename = "";
}

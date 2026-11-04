#include "fable_font_state_blocks.h"

void CStreamingFontBankStateBlock::GetDefault(CBankStateBlock& output)
{
    unsigned char* state = reinterpret_cast<unsigned char*>(&output);
    *reinterpret_cast<unsigned long*>(state + 4) = 0;
    *reinterpret_cast<unsigned long*>(state + 8) = 400;
    *reinterpret_cast<unsigned long*>(state + 0x0c) = 0;
    state[0x10] = 0;
}

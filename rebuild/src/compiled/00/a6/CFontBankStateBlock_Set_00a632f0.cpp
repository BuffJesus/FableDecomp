#include "fable_font_state_blocks.h"

void CFontBankStateBlock::Set(const CBankStateBlock* source)
{
    const unsigned char* input = reinterpret_cast<const unsigned char*>(source);
    unsigned char* output = reinterpret_cast<unsigned char*>(this);
    *reinterpret_cast<unsigned long*>(output + 4) =
        *reinterpret_cast<const unsigned long*>(input + 4);
    *reinterpret_cast<unsigned long*>(output + 8) =
        *reinterpret_cast<const unsigned long*>(input + 8);
    output[0x0c] = input[0x0c];
}

#include "fable_font_state_blocks.h"

void CStreamingFontBankStateBlock::Set(const CBankStateBlock* source)
{
    const unsigned char* input = reinterpret_cast<const unsigned char*>(source);
    unsigned char* output = reinterpret_cast<unsigned char*>(this);
    *reinterpret_cast<unsigned long*>(output + 4) =
        *reinterpret_cast<const unsigned long*>(input + 4);
    *reinterpret_cast<unsigned long*>(output + 8) =
        *reinterpret_cast<const unsigned long*>(input + 8);
    *reinterpret_cast<unsigned long*>(output + 0x0c) =
        *reinterpret_cast<const unsigned long*>(input + 0x0c);
    output[0x10] = input[0x10];
}

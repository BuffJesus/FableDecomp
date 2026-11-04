#include <stdio.h>
#include <string.h>
#include "fable_font_state_blocks.h"

CStreamingFontBankStateBlock::~CStreamingFontBankStateBlock() {}
unsigned long CStreamingFontBankStateBlock::CreateCRC() { return 0; }
void CStreamingFontBankStateBlock::Set(const CBankStateBlock*) {}

int main()
{
    CStreamingFontBankStateBlock bank;
    unsigned char output[0x14];
    memset(output, 0xA5, sizeof(output));
    bank.CStreamingFontBankStateBlock::GetDefault(
        *reinterpret_cast<CBankStateBlock*>(output));
    if (*reinterpret_cast<unsigned long*>(output) != 0xA5A5A5A5 ||
        *reinterpret_cast<unsigned long*>(output + 4) != 0 ||
        *reinterpret_cast<unsigned long*>(output + 8) != 400 ||
        *reinterpret_cast<unsigned long*>(output + 0x0c) != 0 ||
        output[0x10] != 0 || output[0x11] != 0xA5 ||
        output[0x12] != 0xA5 || output[0x13] != 0xA5)
        return 1;
    puts("GETDEFAULT_OK");
    return 0;
}

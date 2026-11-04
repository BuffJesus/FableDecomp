#include <stdio.h>
#include <string.h>
#include "fable_font_state_blocks.h"

CFontBankStateBlock::~CFontBankStateBlock() {}
unsigned long CFontBankStateBlock::CreateCRC() { return 0; }
void CFontBankStateBlock::Set(const CBankStateBlock*) {}

int main()
{
    CFontBankStateBlock bank;
    unsigned char output[0x14];
    memset(output, 0xA5, sizeof(output));
    bank.CFontBankStateBlock::GetDefault(
        *reinterpret_cast<CBankStateBlock*>(output));
    if (*reinterpret_cast<unsigned long*>(output) != 0xA5A5A5A5 ||
        *reinterpret_cast<unsigned long*>(output + 4) != 0 ||
        *reinterpret_cast<unsigned long*>(output + 8) != 400 ||
        output[0x0c] != 0 || output[0x0d] != 0xA5 ||
        output[0x0e] != 0xA5 || output[0x0f] != 0xA5 ||
        output[0x10] != 0xA5)
        return 1;
    puts("OK_00a632d0");
    return 0;
}

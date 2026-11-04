#include <stdio.h>
#include <string.h>
#include "fable_font_state_blocks.h"

CFontBankStateBlock::~CFontBankStateBlock() {}
void CFontBankStateBlock::GetDefault(CBankStateBlock&) {}
unsigned long CFontBankStateBlock::CreateCRC() { return 0; }

struct TestBlock : CFontBankStateBlock
{
    void Apply(const CBankStateBlock* source)
    {
        CFontBankStateBlock::Set(source);
    }
};

int main()
{
    TestBlock destination;
    unsigned char source[0x14];
    unsigned char original[sizeof(source)];
    for (unsigned i = 0; i < sizeof(source); ++i)
        source[i] = static_cast<unsigned char>(0x31 + i * 3);
    memcpy(original, source, sizeof(source));
    memset(destination.State, 0xA5, sizeof(destination.State));
    const void* const vtable = *reinterpret_cast<void**>(&destination);
    destination.Apply(reinterpret_cast<const CBankStateBlock*>(source));
    const unsigned char* result = reinterpret_cast<const unsigned char*>(&destination);
    if (*reinterpret_cast<void* const*>(&destination) != vtable ||
        memcmp(result + 4, source + 4, 8) != 0 ||
        result[0x0c] != source[0x0c] || result[0x0d] != 0xA5 ||
        result[0x0e] != 0xA5 || result[0x0f] != 0xA5 ||
        memcmp(source, original, sizeof(source)) != 0)
        return 1;
    puts("OK_00a632f0");
    return 0;
}

#include <stdio.h>
#include <string.h>
#include "fable_font_state_blocks.h"

CStreamingFontBankStateBlock::~CStreamingFontBankStateBlock() {}
void CStreamingFontBankStateBlock::GetDefault(CBankStateBlock&) {}
unsigned long CStreamingFontBankStateBlock::CreateCRC() { return 0; }

struct TestBlock : CStreamingFontBankStateBlock
{
    void Apply(const CBankStateBlock* source)
    {
        CStreamingFontBankStateBlock::Set(source);
    }
};

int main()
{
    TestBlock destination;
    unsigned char source[0x18];
    unsigned char original[sizeof(source)];
    for (unsigned i = 0; i < sizeof(source); ++i)
        source[i] = static_cast<unsigned char>(0x42 + i * 5);
    memcpy(original, source, sizeof(source));
    memset(destination.State, 0xA5, sizeof(destination.State));
    const void* const vtable = *reinterpret_cast<void**>(&destination);
    destination.Apply(reinterpret_cast<const CBankStateBlock*>(source));
    const unsigned char* result = reinterpret_cast<const unsigned char*>(&destination);
    if (*reinterpret_cast<void* const*>(&destination) != vtable ||
        memcmp(result + 4, source + 4, 13) != 0 ||
        result[0x11] != 0xA5 || result[0x12] != 0xA5 ||
        result[0x13] != 0xA5 ||
        memcmp(source, original, sizeof(source)) != 0)
        return 1;
    puts("OK_00a5ee40");
    return 0;
}

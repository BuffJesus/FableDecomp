#include <stdio.h>
#include <string.h>
#include "fable_crc.h"
#include "fable_font_state_blocks.h"

static const void* g_expectedData;
static const void* g_expectedReceiver;
static unsigned long g_expectedLength;
static unsigned g_calls;
static bool g_valid;

fable_u32 CCRC::Calc(const void* data, fable_u32 byteCount) const
{
    ++g_calls;
    g_valid = this == g_expectedReceiver && data == g_expectedData &&
        byteCount == g_expectedLength;
    const unsigned char* bytes = static_cast<const unsigned char*>(data);
    unsigned long sum = 0;
    for (unsigned i = 0; i < byteCount; ++i)
        sum = sum * 31 + bytes[i];
    return sum;
}

CStreamingFontBankStateBlock::~CStreamingFontBankStateBlock() {}
void CStreamingFontBankStateBlock::GetDefault(CBankStateBlock&) {}
void CStreamingFontBankStateBlock::Set(const CBankStateBlock*) {}

int main()
{
    CStreamingFontBankStateBlock bank;
    for (unsigned i = 0; i < sizeof(bank.State); ++i)
        bank.State[i] = static_cast<unsigned char>(i * 11 + 3);
    g_expectedData = bank.State;
    g_expectedReceiver = &bank;
    g_expectedLength = sizeof(bank.State);
    unsigned long expected = 0;
    for (unsigned i = 0; i < sizeof(bank.State); ++i)
        expected = expected * 31 + bank.State[i];
    if (bank.CStreamingFontBankStateBlock::CreateCRC() != expected ||
        g_calls != 1 || !g_valid)
        return 1;
    puts("CRC_STATE_BLOCKS PASS");
    return 0;
}

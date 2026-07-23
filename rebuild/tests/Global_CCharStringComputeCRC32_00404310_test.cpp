#include <stdio.h>

struct CharStringStorage
{
    const void* data;
    unsigned long length;
};

struct CCharStringOverlay
{
    CharStringStorage* storage;
};

class CCRC
{
public:
    static unsigned long __fastcall Calc(
        unsigned long seed, const void* data, unsigned long length);
};

unsigned char DAT_0129aaf4;
static unsigned long g_seed;
static const void* g_data;
static unsigned long g_length;
static unsigned long g_result;

unsigned long __fastcall CCRC::Calc(
    unsigned long seed, const void* data, unsigned long length)
{
    g_seed = seed;
    g_data = data;
    g_length = length;
    return g_result;
}

unsigned long __fastcall CCharString_ComputeCRC32(CCharStringOverlay*);

int main()
{
    CCharStringOverlay value;
    value.storage = 0;
    g_result = 0x12345678UL;
    if (CCharString_ComputeCRC32(&value) != g_result) return 1;
    if (g_seed != 0 || g_data != &DAT_0129aaf4 || g_length != 0) return 2;

    const char bytes[] = "fable";
    CharStringStorage storage = { bytes, 5 };
    value.storage = &storage;
    g_result = 0x9abcdef0UL;
    if (CCharString_ComputeCRC32(&value) != g_result) return 3;
    if (g_seed != 0 || g_data != bytes || g_length != 5) return 4;

    printf("CCHARSTRING_COMPUTE_CRC32_TEST PASS\n");
    return 0;
}

// Behaviour fixture for the CWADBankStateBlock overrides 0x00A3A2C0..0x00A3A2E3, linked against the provider object:
// a WAD bank has no load state, so GetDefault/Set leave both blocks untouched and CreateCRC is 0.
#include "fable_wad_file.h"
#include <stdio.h>
#include <string.h>

struct CWADBankStateBlockTest : CWADBankStateBlock
{
    void CallSet(const CBankStateBlock* source) { CWADBankStateBlock::Set(source); }
};

int main()
{
    static unsigned char a[sizeof(CWADBankStateBlock)], b[sizeof(CWADBankStateBlock)];
    memset(a, 0x5A, sizeof(a));
    memset(b, 0xA5, sizeof(b));
    CWADBankStateBlockTest* block = reinterpret_cast<CWADBankStateBlockTest*>(a);
    CWADBankStateBlock* other = reinterpret_cast<CWADBankStateBlock*>(b);
    block->CWADBankStateBlock::GetDefault(*other);
    block->CallSet(other);
    const unsigned long crc = block->CWADBankStateBlock::CreateCRC();
    bool ok = crc == 0;
    for (size_t i = 0; i < sizeof(a); ++i)
        if (a[i] != 0x5A || b[i] != 0xA5) ok = false;
    if (ok)
    {
        printf("CWADBANKSTATEBLOCK_OK\n");
        return 0;
    }
    printf("FAIL crc=%lu\n", crc);
    return 1;
}

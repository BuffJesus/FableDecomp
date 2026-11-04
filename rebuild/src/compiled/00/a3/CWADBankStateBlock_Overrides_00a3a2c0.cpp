// CWADBankStateBlock overrides (retail 0x00A3A2C0..0x00A3A2E3): a WAD bank keeps no load state.
#include "fable_wad_file.h"

// 0x00A3A2C0
void CWADBankStateBlock::GetDefault(CBankStateBlock& output)
{
}

// 0x00A3A2D0
unsigned long CWADBankStateBlock::CreateCRC()
{
    return 0;
}

// 0x00A3A2E0
void CWADBankStateBlock::Set(const CBankStateBlock* source)
{
}

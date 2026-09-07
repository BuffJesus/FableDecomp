#include "engine/CStreamingFontBankStateBlock.h"  // retyped onto the PDB layout; byte parity re-verified
struct CBankStateBlock {
    int f0;
    int f4;
    int f8;
    int fC;
    char f10;
};

struct CStreamingFontBankStateBlock_Methods : CStreamingFontBankStateBlock {
    void GetDefault(CBankStateBlock &out);
};

void CStreamingFontBankStateBlock_Methods::GetDefault(CBankStateBlock &out)
{
    out.f4 = 0;
    out.f8 = 0x190;
    out.fC = 0;
    out.f10 = 0;
}
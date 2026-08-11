struct CBankStateBlock {
    int f0;
    int f4;
    int f8;
    int fC;
    char f10;
};

struct CStreamingFontBankStateBlock {
    void GetDefault(CBankStateBlock &out);
};

void CStreamingFontBankStateBlock::GetDefault(CBankStateBlock &out)
{
    out.f4 = 0;
    out.f8 = 0x190;
    out.fC = 0;
    out.f10 = 0;
}
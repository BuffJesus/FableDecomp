struct CFontBankStateBlock {
    int   f0;
    int   f4;
    int   f8;
    char  fc;
};

void __stdcall GetDefault(CFontBankStateBlock* p)
{
    p->f4 = 0;
    p->f8 = 0x190;
    p->fc = 0;
}
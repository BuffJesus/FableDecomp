struct Src {
    char pad0[0x71];
    unsigned char b71;
    char pad72[2];
    int  d74;
};

struct CShaderBankStateBlock {
    char pad0[8];
    unsigned char b8;
    char pad9[0x12f8 - 9];
    int  d12f8;
};

void __fastcall Set(CShaderBankStateBlock* self, int /*edx*/, void* srcv)
{
    Src* src = (Src*)srcv;
    self->b8    = src->b71;
    self->d12f8 = src->d74;
}
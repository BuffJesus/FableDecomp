struct Sub;

extern void __fastcall Sub_Do(Sub* self, int a, unsigned char b);

struct CTCVillage {
    Sub* m0;        // +0
    int  m4;        // +4
    unsigned char m8;
    unsigned char m9;
    unsigned char ma;
    unsigned char mb;
    unsigned char mC;   // +0xc
    unsigned char mD;   // +0xd
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->mD) {
        Sub_Do((Sub*)self->m0, self->m4, self->mC);
        self->mD = 0;
    }
}
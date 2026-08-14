struct Sub;

// callee at 0xa84cf0: this in ecx, edx arg, one stack arg
extern void __fastcall Sub_Handle(Sub* thisp, int edxArg, unsigned char stackArg);

struct CTCVillage {
    Sub*          f0;   // +0x00
    int           f4;   // +0x04
    // ... padding to 0x0c
    char          pad8[4];
    unsigned char f0c;  // +0x0c
    unsigned char f0d;  // +0x0d
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->f0d) {
        Sub_Handle(self->f0, self->f4, self->f0c);
        self->f0d = 0;
    }
}
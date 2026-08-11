// CMeleeApprentice::Init @ 0x00ded0a0
// Zero 7 bytes at f48,f49,f4a,f4b,f4c,f4d,f4f (skips 0x4e). Shared al=0.

struct CMeleeApprentice {
    char pad[0x48];
    unsigned char f48; // 0x48
    unsigned char f49; // 0x49
    unsigned char f4a; // 0x4a
    unsigned char f4b; // 0x4b
    unsigned char f4c; // 0x4c
    unsigned char f4d; // 0x4d
    unsigned char f4e; // 0x4e (skipped)
    unsigned char f4f; // 0x4f
};

void __fastcall CMeleeApprentice_Init(CMeleeApprentice *self)
{
    self->f48 = 0;
    self->f49 = 0;
    self->f4a = 0;
    self->f4b = 0;
    self->f4c = 0;
    self->f4d = 0;
    self->f4f = 0;
}
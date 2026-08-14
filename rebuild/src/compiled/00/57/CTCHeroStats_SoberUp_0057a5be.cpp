#pragma optimize("s",on)
struct CTCHeroStats {
    char pad[0x19c];
    float f19c;   // +0x19c
    float f1a0;   // +0x1a0
    char pad2[0x1ac - 0x1a4];
    int   i1ac;   // +0x1ac
    char pad3[0x1b8 - 0x1b0];
    unsigned char b1b8; // +0x1b8
};

void __fastcall SoberUp(CTCHeroStats* self)
{
    self->i1ac = -1;
    self->f19c = 0.0f;
    self->b1b8 = 1;
    self->f1a0 = 0.0f;
}
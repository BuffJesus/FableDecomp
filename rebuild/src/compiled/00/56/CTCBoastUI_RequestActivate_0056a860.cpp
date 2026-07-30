struct CTCBoastUI {
    char pad0[0x18];
    unsigned char f18;
    char pad19[0x0f];
    unsigned char f28;
    unsigned char f29;
    unsigned char f2a;
};

void __fastcall RequestActivate(CTCBoastUI *self)
{
    if (!self->f2a && !self->f18 && !self->f28)
        self->f2a = 1;
}
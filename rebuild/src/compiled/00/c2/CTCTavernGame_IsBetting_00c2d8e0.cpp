struct CTCTavernGame {
    unsigned int pad0;   // +0
    unsigned int a;      // +4
    unsigned int b;      // +8
};

bool __fastcall IsBetting(CTCTavernGame *self)
{
    return self->a > 0 || self->b > 0;
}
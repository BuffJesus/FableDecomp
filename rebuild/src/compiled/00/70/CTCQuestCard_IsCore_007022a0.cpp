
struct QCard {
    char pad0[0xc];
    unsigned char* f0c;
    int pad10;
    int f14;
};

bool __fastcall IsCore_2(QCard* self)
{
    if (self->f14 <= -1)
    {
        if (self->f0c[0x5a] != 0)
            return true;
    }
    return false;
}
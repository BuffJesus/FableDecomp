struct CTCInner
{
    char pad[0x25];
    char flag;      // +0x25
};

struct CTCTurncoat
{
    char pad0[0x14];
    CTCInner* inner;   // +0x14
    char pad1[0x2c - 0x18];
    char enabled;      // +0x2c

    int IsTurncoatable();
};

int CTCTurncoat::IsTurncoatable()
{
    if (this->enabled)
    {
        CTCInner* p = this->inner;
        if (p->flag)
            return 1;
    }
    return 0;
}
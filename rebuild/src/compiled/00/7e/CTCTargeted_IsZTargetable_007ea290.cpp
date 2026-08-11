struct CTCTargeted {
    char pad0[0x0c];
    unsigned int f0c;   // +0x0c
    unsigned char f10;  // +0x10
};

bool __fastcall IsZTargetable(CTCTargeted *self)
{
    unsigned int v = self->f0c;
    if (v != 0 && self->f10 != 0 && (v & 0x20) != 0)
        return true;
    return false;
}
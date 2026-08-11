struct CThing {
    char pad00[0x2c];
    unsigned char f2c;   // +0x2c flags byte
    char pad2d[0x5c-0x2d];
    void* f5c;           // +0x5c
};

bool __fastcall GetTCDriver_CTCDExperienceOrb(CThing* self, int /*edx*/, void** out)
{
    if (self->f2c & 0x10) {
        *out = self->f5c;
        return true;
    }
    return false;
}
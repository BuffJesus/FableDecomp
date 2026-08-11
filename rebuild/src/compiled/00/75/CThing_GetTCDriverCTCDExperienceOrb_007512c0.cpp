struct CThing {
    char pad0[0x28];
    unsigned char f28;   // 0x28
    char pad1[0x5c-0x29];
    void* f5c;           // 0x5c
};

bool __fastcall GetTCDriver_CTCDExperienceOrb(CThing* self, int /*edx*/, void** out)
{
    if (self->f28 & 2) {
        *out = self->f5c;
        return true;
    }
    return false;
}
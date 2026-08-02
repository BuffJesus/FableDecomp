struct CTCPhysicsStandard
{
    char pad[0x1c8];
    unsigned char flags1c8;
};

bool __fastcall CTCPhysicsStandard_CanCollideWithLandscape(CTCPhysicsStandard* self, void* /*edx*/)
{
    unsigned char v = self->flags1c8;
    v = v >> 3;
    v = v & 1;
    return v;
}
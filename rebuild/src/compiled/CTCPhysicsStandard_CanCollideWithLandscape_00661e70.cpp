struct CTCPhysicsStandard { char pad[0x15d]; unsigned char flags; };

unsigned char __fastcall CTCPhysicsStandard_CanCollideWithLandscape(const CTCPhysicsStandard* self)
{
    unsigned char v = self->flags;
    v = (unsigned char)(v >> 2);
    v = (unsigned char)(v & 1);
    return v;
}
// CThing::SetAsGameStatePersisted @ 0x00661ea0
// void __fastcall (modeled as __fastcall self in ecx), param bool.
// Sets bit 1 (value 0x2) of the flags byte at +0x15d to match the bool arg,
// preserving all other bits, via the toggle idiom flags ^= ((param<<1) ^ flags) & 2.

struct CThing {
    char pad[0x15d];
    unsigned char flags; // at +0x15d
};

void __fastcall CThing_SetAsGameStatePersisted(CThing* self, int /*edx*/, bool persisted)
{
    unsigned char v = ((unsigned char)persisted << 1);
    v = (unsigned char)((v ^ self->flags) & 2);
    self->flags = self->flags ^ v;
}
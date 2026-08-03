// CThing::SetAsGameStatePersisted  0x004c7770
// Sets bit 3 (0x08) of the byte bitfield at this+0x91 from a bool param.
struct CThing {
    char pad[0x91];
    unsigned char flags;   // +0x91
};

void __fastcall SetAsGameStatePersisted(CThing* self, void* /*edx*/, bool persisted)
{
    unsigned char v = (unsigned char)((unsigned char)persisted << 3);
    self->flags ^= ((v ^ self->flags) & 0x08);
}
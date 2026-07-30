// CThing::IsIncludedInThingScans @ 0x0048bc40
// Reads flags byte at this+0x91. Returns 1 iff bits: 0x04 set, 0x08 set, 0x01 clear, 0x20 clear.
struct CThing {
    char pad[0x91];
    unsigned char flags;
};

int __fastcall IsIncludedInThingScans(CThing *self)
{
    unsigned char f = self->flags;
    unsigned char nf = ~f;
    if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
        return 1;
    return 0;
}
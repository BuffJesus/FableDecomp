// CTCPhysicsStandard::CanCollideWithLandscape — bit 5 of the byte at this+0x1c8.
// Keep the arithmetic 8-bit (unsigned char in AND out): promoting to int makes VC7.1
// emit `shr eax,5 / and eax,1` instead of retail's byte-width `shr al,5 / and al,1`.
#pragma pack(push,1)
struct T { char pad[0x1c8]; unsigned char flags; unsigned char CanCollide(); };
#pragma pack(pop)
unsigned char T::CanCollide() {
    unsigned char f = this->flags;
    f >>= 5;
    f &= 1;
    return f;
}
struct CTCPhysicsStandard
{
    char pad_0000[0x1c8];
    unsigned char flags1c8;

    unsigned char CanCollideWithLandscape_a();
};

unsigned char CTCPhysicsStandard::CanCollideWithLandscape_a()
{
    unsigned char v = flags1c8;
    v = v >> 2;
    v = v & 1;
    return v;
}
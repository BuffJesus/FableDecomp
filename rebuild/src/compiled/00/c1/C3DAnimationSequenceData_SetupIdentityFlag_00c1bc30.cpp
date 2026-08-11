struct C3DAnimationSequenceData
{
    unsigned char pad0;
    unsigned char pad1;
    unsigned char flags;
    void SetupIdentityFlag(bool on);
};

void C3DAnimationSequenceData::SetupIdentityFlag(bool on)
{
    if (on)
        flags |= 0x02;
    else
        flags &= 0xFD;
}
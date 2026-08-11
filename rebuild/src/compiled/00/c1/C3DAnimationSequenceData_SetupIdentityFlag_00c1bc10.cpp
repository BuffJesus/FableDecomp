struct C3DAnimationSequenceData {
    char pad0;
    char pad1;
    unsigned char flags;
    void SetupIdentityFlag(bool set);
};

void C3DAnimationSequenceData::SetupIdentityFlag(bool set)
{
    if (set)
        flags = (unsigned char)(flags | 1);
    else
        flags = (unsigned char)(flags & 0xFE);
}
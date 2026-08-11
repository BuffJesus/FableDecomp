struct C3DAnimationSequenceData {
    char pad0;
    char pad1;
    unsigned char flags; // +2
    void SetupIdentityFlag(bool enable);
};

void C3DAnimationSequenceData::SetupIdentityFlag(bool enable)
{
    if (enable)
        this->flags |= 4;
    else
        this->flags &= 0xFB;
}
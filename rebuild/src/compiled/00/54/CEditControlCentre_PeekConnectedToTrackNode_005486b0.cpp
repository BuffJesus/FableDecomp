struct CEditControlCentre {
    char pad0[0xC];
    unsigned char connectedFlag;
    char pad1[0x48 - 0xC - 1];
    void* trackNodeA;
    char pad2[0x104 - 0x48 - 4];
    void* trackNodeB;
};

void* __fastcall PeekConnectedToTrackNode(CEditControlCentre* self)
{
    if (self->connectedFlag)
        return self->trackNodeB;
    return self->trackNodeA;
}
// CThing::SetAsGameStatePersisted  retail 0x004c7880  __fastcall void(CThing*, bool)
struct CThing {
    char pad[0x92];
    unsigned char flags;   // byte at +0x92
    void SetAsGameStatePersisted(bool persisted);
};

void CThing::SetAsGameStatePersisted(bool persisted)
{
    unsigned char t = (unsigned char)(persisted << 5);
    t ^= this->flags;
    t &= 0x20;
    this->flags ^= t;
}
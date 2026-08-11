#include <stdio.h>

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

int main()
{
    C3DAnimationSequenceData d;
    d.flags = 0x01;
    d.SetupIdentityFlag(true);
    bool ok = (d.flags == 0x05);
    d.SetupIdentityFlag(false);
    ok = ok && (d.flags == 0x01);

    d.flags = 0xFF;
    d.SetupIdentityFlag(false);
    ok = ok && (d.flags == 0xFB);
    d.SetupIdentityFlag(true);
    ok = ok && (d.flags == 0xFF);

    if (ok) printf("SETUPIDENTITY_OK\n");
    else printf("FAIL flags=%02x\n", d.flags);
    return ok ? 0 : 1;
}
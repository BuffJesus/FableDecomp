#include <cstdio>

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

int main()
{
    C3DAnimationSequenceData d;
    d.pad0 = 0xAA; d.pad1 = 0xBB; d.flags = 0x01;

    d.SetupIdentityFlag(true);
    if (d.flags != 0x03) { printf("FAIL set %02x\n", d.flags); return 1; }

    d.SetupIdentityFlag(false);
    if (d.flags != 0x01) { printf("FAIL clear %02x\n", d.flags); return 1; }

    d.flags = 0xFF;
    d.SetupIdentityFlag(false);
    if (d.flags != 0xFD) { printf("FAIL clear2 %02x\n", d.flags); return 1; }

    if (d.pad0 != 0xAA || d.pad1 != 0xBB) { printf("FAIL pad\n"); return 1; }

    printf("IDENTITY_FLAG_OK\n");
    return 0;
}
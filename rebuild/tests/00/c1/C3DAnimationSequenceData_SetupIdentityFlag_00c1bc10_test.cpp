#include <cstdio>

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

int main()
{
    C3DAnimationSequenceData d;
    d.pad0 = 0; d.pad1 = 0; d.flags = 0xF0;

    d.SetupIdentityFlag(true);
    if (d.flags != 0xF1) { printf("FAIL set\n"); return 1; }

    d.SetupIdentityFlag(false);
    if (d.flags != 0xF0) { printf("FAIL clear\n"); return 1; }

    d.flags = 0x01;
    d.SetupIdentityFlag(true);
    if (d.flags != 0x01) { printf("FAIL setidem\n"); return 1; }

    d.SetupIdentityFlag(false);
    if (d.flags != 0x00) { printf("FAIL clearidem\n"); return 1; }

    printf("IDENTITYFLAG_OK\n");
    return 0;
}
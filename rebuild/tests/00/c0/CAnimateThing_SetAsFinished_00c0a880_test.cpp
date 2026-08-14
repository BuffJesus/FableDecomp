#include <cstdio>
#include <cstring>

struct CAnimateThing {
    unsigned char pad[0x100];
};

void SetAsFinished_model(CAnimateThing *self, bool finished)
{
    unsigned char *p = (unsigned char *)self;
    p[0x26] |= 0x80;
    p[0x54] = (unsigned char)finished;
}

int main()
{
    CAnimateThing t;
    memset(&t, 0, sizeof(t));
    t.pad[0x26] = 0x01;
    SetAsFinished_model(&t, true);
    if (t.pad[0x26] != 0x81) { printf("FAIL 26=%02x\n", t.pad[0x26]); return 1; }
    if (t.pad[0x54] != 0x01) { printf("FAIL 54=%02x\n", t.pad[0x54]); return 1; }

    memset(&t, 0xFF, sizeof(t));
    SetAsFinished_model(&t, false);
    if (t.pad[0x26] != 0xFF) { printf("FAIL 26=%02x\n", t.pad[0x26]); return 1; }
    if (t.pad[0x54] != 0x00) { printf("FAIL 54=%02x\n", t.pad[0x54]); return 1; }

    printf("BEHAVIOR_OK\n");
    return 0;
}
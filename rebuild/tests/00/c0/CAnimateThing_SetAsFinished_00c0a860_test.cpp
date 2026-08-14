#include <cstdio>

struct CAnimateThing {
    unsigned char b[0x50];
};

void __fastcall SetAsFinished(CAnimateThing *self, int /*edx*/, void *value)
{
    *((unsigned char *)self + 0x26) |= 0x20;
    *(void **)((char *)self + 0x48) = value;
}

int main()
{
    CAnimateThing t;
    for (int i = 0; i < 0x50; ++i) t.b[i] = 0;
    t.b[0x26] = 0x01; // pre-existing bits must be preserved

    int marker = 12345;
    SetAsFinished(&t, 0, &marker);

    bool ok = true;
    if (t.b[0x26] != (0x01 | 0x20)) ok = false;           // OR set bit 0x20, kept 0x01
    void *stored = *(void **)(&t.b[0x48]);
    if (stored != (void *)&marker) ok = false;            // value stored at +0x48

    if (ok) printf("PARITY_OK\n");
    else    printf("PARITY_FAIL b26=%02x stored=%p want=%p\n", t.b[0x26], stored, (void*)&marker);
    return 0;
}
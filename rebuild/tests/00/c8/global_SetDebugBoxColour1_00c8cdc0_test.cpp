#include <cstdio>

struct ColourBytes {
    unsigned char pad[0xe];
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    unsigned char b3;
};
struct CEngineWeatherRenderer {
    ColourBytes* data;
    unsigned int SetDebugBoxColour1();
};

int main()
{
    ColourBytes cb;
    for (int i = 0; i < 0xe; ++i) cb.pad[i] = 0xAA;
    cb.b0 = 0x11; // +0xe
    cb.b1 = 0x22; // +0xf
    cb.b2 = 0x33; // +0x10
    cb.b3 = 0x44; // +0x11
    CEngineWeatherRenderer r;
    r.data = &cb;

    unsigned int got = r.SetDebugBoxColour1();
    unsigned int want = 0x44332211u;
    if (got != want) { printf("FAIL got=%08X want=%08X\n", got, want); return 1; }

    cb.b0 = 0xDE; cb.b1 = 0xAD; cb.b2 = 0xBE; cb.b3 = 0xEF;
    got = r.SetDebugBoxColour1();
    if (got != 0xEFBEADDEu) { printf("FAIL2 got=%08X\n", got); return 1; }

    printf("COLOUR1_OK\n");
    return 0;
}
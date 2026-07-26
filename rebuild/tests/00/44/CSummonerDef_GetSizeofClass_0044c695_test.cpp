#include <cstdio>

typedef unsigned long ulong;

ulong __fastcall CSummonerDef_GetSizeofClass(void* self)
{
    (void)self;
    return 0x5c;
}

int main()
{
    ulong r = CSummonerDef_GetSizeofClass(0);
    if (r == 0x5c) {
        std::printf("CSummonerDef_0044c695_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}
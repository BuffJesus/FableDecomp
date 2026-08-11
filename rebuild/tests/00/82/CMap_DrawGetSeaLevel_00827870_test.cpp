#include <cstdio>

struct CMap {
    char pad[0x1d08];
    float seaLevel; // +0x1d08
};

float __fastcall DrawGetSeaLevel(CMap* self, int, int);

int main()
{
    CMap m;
    m.seaLevel = 42.5f;
    float r = DrawGetSeaLevel(&m, 0, 0);
    if (r == 42.5f)
        printf("00827870_TEST PASS\n");
    else
        printf("FAIL %f\n", r);
    return 0;
}
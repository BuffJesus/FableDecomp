#include <stdio.h>

struct CMap {
    char pad00[0x34];
    float f34;
    char pad38[0x14];
    float f4c;
};

float __fastcall DrawGetSeaLevel(CMap *thisptr)
{
    return thisptr->f4c * thisptr->f34;
}

int main(void)
{
    CMap m;
    m.f34 = 2.5f;    // scale
    m.f4c = 4.0f;    // sea level
    float r = DrawGetSeaLevel(&m);
    if (r == 10.0f) {
        printf("OK_0x004bbff0 result=%f\n", r);
        return 0;
    }
    printf("FAIL result=%f\n", r);
    return 1;
}
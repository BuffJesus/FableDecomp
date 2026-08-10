#include <stdio.h>

struct C2DBoxI { int x0, y0, x1, y1; };

struct CWorldMap {
    char pad[0x5c];
    C2DBoxI dimensions;
};

C2DBoxI const & __fastcall GetDimensions(CWorldMap *self)
{
    return self->dimensions;
}

int main()
{
    CWorldMap m;
    m.dimensions.x0 = 1;
    m.dimensions.y0 = 2;
    m.dimensions.x1 = 3;
    m.dimensions.y1 = 4;
    C2DBoxI const & r = GetDimensions(&m);
    if (&r == &m.dimensions && r.x0 == 1 && r.y1 == 4)
        printf("0051cde0_TEST PASS\n");
    else
        printf("FAIL\n");
    return 0;
}
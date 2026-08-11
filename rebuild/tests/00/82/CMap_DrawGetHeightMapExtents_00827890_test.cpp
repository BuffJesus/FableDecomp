#include <cstdio>

struct C2DExtentsI { int a; int b; };

struct CMap {
    char pad[0x1d14];
    int f1d14;
    int f1d18;
    C2DExtentsI DrawGetHeightMapExtents();
};

C2DExtentsI CMap::DrawGetHeightMapExtents()
{
    C2DExtentsI r;
    r.b = this->f1d18;
    r.a = this->f1d14;
    return r;
}

int main()
{
    CMap m;
    m.f1d14 = 111;
    m.f1d18 = 222;
    C2DExtentsI e = m.DrawGetHeightMapExtents();
    if (e.a == 111 && e.b == 222)
        printf("00827890_TEST PASS\n");
    else
        printf("FAIL %d %d\n", e.a, e.b);
    return 0;
}
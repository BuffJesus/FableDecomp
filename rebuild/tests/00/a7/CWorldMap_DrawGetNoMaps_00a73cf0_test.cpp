#include <cstdio>

struct MapEntry6 {
    unsigned short a;
    unsigned short b;
    unsigned short c;
    unsigned short d;
    unsigned short e;
    unsigned short f;
};

struct CWorldMap {
    char pad[0x48];
    MapEntry6* begin; // +0x48
    MapEntry6* end;   // +0x4c
    long DrawGetNoMaps();
};

long CWorldMap::DrawGetNoMaps()
{
    return (long)(this->end - this->begin);
}

int main()
{
    MapEntry6 buf[10];
    CWorldMap m;
    m.begin = buf;
    m.end = buf + 7;
    long n = m.DrawGetNoMaps();

    CWorldMap m0;
    m0.begin = buf;
    m0.end = buf;
    long z = m0.DrawGetNoMaps();

    if (n == 7 && z == 0) {
        printf("MAPCOUNT_OK n=%ld z=%ld\n", n, z);
        return 0;
    }
    printf("FAIL n=%ld z=%ld\n", n, z);
    return 1;
}
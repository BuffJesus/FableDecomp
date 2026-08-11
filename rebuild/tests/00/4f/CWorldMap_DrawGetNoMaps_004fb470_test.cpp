#include <stdio.h>

struct MapEntry { char pad[88]; };

struct CWorldMap {
    char pad0[0x2c];
    MapEntry* begin;   // +0x2c
    MapEntry* end;     // +0x30
    long DrawGetNoMaps();
};

long CWorldMap::DrawGetNoMaps()
{
    return (long)(this->end - this->begin);
}

int main()
{
    MapEntry buf[10];
    CWorldMap m;
    m.begin = &buf[0];

    m.end = &buf[0];
    long r0 = m.DrawGetNoMaps();

    m.end = &buf[7];
    long r7 = m.DrawGetNoMaps();

    m.end = &buf[10];
    long r10 = m.DrawGetNoMaps();

    if (r0 == 0 && r7 == 7 && r10 == 10) {
        printf("DRAWGETNOMAPS_OK %ld %ld %ld\n", r0, r7, r10);
        return 0;
    }
    printf("FAIL %ld %ld %ld\n", r0, r7, r10);
    return 1;
}
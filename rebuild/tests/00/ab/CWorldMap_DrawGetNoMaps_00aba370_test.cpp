#include <cstdio>

struct MapEntry6 { char raw[6]; };

struct CWorldMap {
    char pad0[0x2c];
    MapEntry6* begin;   // +0x2c
    MapEntry6* end;     // +0x30
};

long __fastcall DrawGetNoMaps(CWorldMap* self);

int main()
{
    MapEntry6 buf[10];
    CWorldMap w;
    w.begin = &buf[0];
    w.end   = &buf[7];   // 7 elements
    long n = DrawGetNoMaps(&w);
    if (n != 7) { printf("FAIL got %ld\n", n); return 1; }

    w.begin = &buf[3];
    w.end   = &buf[3];
    if (DrawGetNoMaps(&w) != 0) { printf("FAIL empty\n"); return 1; }

    printf("00aba370_TEST PASS\n");
    return 0;
}
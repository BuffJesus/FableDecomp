#include <cstdio>

struct CWorldMap {
    char  pad_0[0x1c];
    char* mapsBegin;
    char* mapsEnd;
    long DrawGetNoMaps();
};

long CWorldMap::DrawGetNoMaps()
{
    return (long)((this->mapsEnd - this->mapsBegin) / 72);
}

int main()
{
    char buf[72 * 10];
    CWorldMap w;
    w.mapsBegin = &buf[0];

    int ok = 1;

    w.mapsEnd = &buf[0];
    if (w.DrawGetNoMaps() != 0) ok = 0;

    w.mapsEnd = &buf[72 * 7];
    if (w.DrawGetNoMaps() != 7) ok = 0;

    w.mapsEnd = &buf[72 * 10];
    if (w.DrawGetNoMaps() != 10) ok = 0;

    w.mapsBegin = &buf[72 * 5];
    w.mapsEnd = &buf[72 * 2];
    if (w.DrawGetNoMaps() != -3) ok = 0;

    if (ok) printf("NOMAPS_OK\n");
    else    printf("NOMAPS_FAIL\n");
    return ok ? 0 : 1;
}
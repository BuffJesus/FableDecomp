#include <stdio.h>

struct C2DCoordI { long x; long y; };
struct MapEntry { long a; long b; long pad[16]; };
struct CWorldMap { long f[8]; MapEntry* f20; };

C2DCoordI __fastcall GetMapPos(CWorldMap* self, long index)
{
    C2DCoordI r;
    MapEntry* e = self->f20 + index;
    r.x = e->a;
    r.y = e->b;
    return r;
}

int main()
{
    MapEntry arr[4];
    for (int i = 0; i < 4; i++) {
        arr[i].a = 100 + i;
        arr[i].b = 200 + i;
        for (int j = 0; j < 16; j++) arr[i].pad[j] = -1;
    }
    CWorldMap m;
    m.f20 = arr;

    int ok = 1;
    for (int i = 0; i < 4; i++) {
        C2DCoordI c = GetMapPos(&m, i);
        if (c.x != 100 + i || c.y != 200 + i) ok = 0;
    }
    // verify stride is 72 bytes (entry index i reads arr[i])
    if (sizeof(MapEntry) != 72) ok = 0;

    if (ok) printf("PARITY_OK\n");
    else printf("PARITY_FAIL\n");
    return 0;
}
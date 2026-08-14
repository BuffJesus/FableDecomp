// CWorldMap::GetMapPos  @ 0x004fc690  __fastcall, returns C2DCoordI by value
struct C2DCoordI {
    long x;
    long y;
    C2DCoordI() {}
    C2DCoordI(long ix, long iy) : x(ix), y(iy) {}
};

struct MapEntry {
    long a;       // +0x00
    long b;       // +0x04
    long pad[16]; // stride = 72 bytes (index scaled by 9*8)
};

struct CWorldMap {
    long f0;
    long f4;
    long f8;
    long fc;
    long f10;
    long f14;
    long f18;
    long f1c;
    MapEntry* f20;   // +0x20

    C2DCoordI GetMapPos(long index);
};

C2DCoordI CWorldMap::GetMapPos(long index)
{
    return C2DCoordI(this->f20[index].a, this->f20[index].b);
}
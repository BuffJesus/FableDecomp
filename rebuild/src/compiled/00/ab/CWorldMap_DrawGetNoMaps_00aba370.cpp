// CWorldMap::DrawGetNoMaps @ 0x00aba370
// Returns number of 6-byte-stride elements between two pointers at +0x2c and +0x30.

struct MapEntry6 { char raw[6]; };

struct CWorldMap {
    char pad0[0x2c];
    MapEntry6* begin;   // +0x2c
    MapEntry6* end;     // +0x30
};

long __fastcall DrawGetNoMaps(CWorldMap* self)
{
    return (long)(self->end - self->begin);
}
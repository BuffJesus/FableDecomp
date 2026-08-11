// CWorldMap::DrawGetNoMaps  retail 0x004fb470
// Returns the number of map entries: (end - begin) / sizeof(entry), entry size = 88 bytes.
// Fields: +0x2c = begin, +0x30 = end.

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
// CWorldMap::DrawGetNoMaps @ 004fb4b0
// long __fastcall(CWorldMap*): count of elements (size 72) between two pointers
// at +0x1c (begin) and +0x20 (end). Modeled as a real member so 'this' is in ecx.

struct CWorldMap {
    char  pad_0[0x1c];   // 0x00 .. 0x1b
    char* mapsBegin;     // 0x1c
    char* mapsEnd;       // 0x20
    long DrawGetNoMaps();
};

long CWorldMap::DrawGetNoMaps()
{
    return (long)((this->mapsEnd - this->mapsBegin) / 72);
}
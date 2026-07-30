#pragma optimize("s",on)
// CWorldMap::DrawGetNoMaps @ 0x005bce2f
// Returns number of elements in a vector-like span [+0x20, +0x24), element size 0x2c.
struct CWorldMap {
    char pad[0x20];
    int* begin;  // +0x20
    int* end;    // +0x24
};

int __fastcall DrawGetNoMaps(CWorldMap* this_)
{
    return (int)((char*)this_->end - (char*)this_->begin) / 0x2c;
}
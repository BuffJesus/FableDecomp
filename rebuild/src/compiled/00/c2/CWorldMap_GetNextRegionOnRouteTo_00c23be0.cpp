// Byte-exact reconstruction of CWorldMap::GetNextRegionOnRouteTo @ 0x00c23be0
struct CWorldMap {
    char pad0[8];
    long count;   // +0x08
    char pad2[8];
    long* array;  // +0x14
};

// member fn @ 0xc23c00 (__fastcall, returns long index)
extern long __fastcall CWorldMap_helper(CWorldMap* self);

long __fastcall GetNextRegionOnRouteTo(CWorldMap* self)
{
    if (self->count == 0)
        return 0;
    long idx = CWorldMap_helper(self);
    long r = self->array[idx];
    self->count = self->count - 1;
    return r;
}
// Byte-exact reconstruction of CWorldMap::GetNextRegionOnRouteTo @ 0x00c24020
// __fastcall(self); harness rewrites __fastcall->__fastcall.
// Layout: +0x08 = count (long), +0x14 = long* array base.

struct CWorldMap {
    char pad0[8];
    long count;      // +0x08
    char pad1[8];    // +0x0C..0x13
    long* arr;       // +0x14
};

// callee member at 0xc24040, takes self in ecx; return value ignored but
// leaves the count value live in eax so the caller reuses it as index.
long __fastcall CWorldMap_helper(CWorldMap* self);

long __fastcall CWorldMap_GetNextRegionOnRouteTo(CWorldMap* self)
{
    long i = self->count;
    if (i == 0)
        return 0;
    i = CWorldMap_helper(self);
    long r = self->arr[i];
    self->count = self->count - 1;
    return r;
}
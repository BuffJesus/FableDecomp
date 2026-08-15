// Byte-exact reconstruction of CWorldMap::GetNextRegionOnRouteTo @ 0x00c24280
// __fastcall (this in ecx). Params (long,long) are unused -> epilogue is plain `ret`,
// so model with only the this pointer (ecx) to avoid ret N cleanup.

struct CWorldMap;

// helper at 0x00c242a0, this-call, returns an index in eax
long __fastcall CWorldMap_ComputeIndex(CWorldMap* self);

struct CWorldMap {
    char pad0[8];
    long count;   // +0x8
    char pad2[8]; // +0xC..+0x13
    long* arr;    // +0x14
};

long __fastcall GetNextRegionOnRouteTo(CWorldMap* self)
{
    if (self->count == 0)
        return 0;
    long idx = CWorldMap_ComputeIndex(self);
    long result = self->arr[idx];
    self->count = self->count - 1;
    return result;
}
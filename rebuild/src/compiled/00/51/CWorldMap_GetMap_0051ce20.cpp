// CWorldMap::GetMap @ 0x0051ce20
// Retail bytes: 8b 44 24 04  8b 49 20  8d 04 c0  8b 44 c1 10  c2 04 00
//   mov eax,[esp+4]          ; index arg
//   mov ecx,[ecx+0x20]       ; this->m_maps (base of entry array)
//   lea eax,[eax+eax*8]      ; index*9
//   mov eax,[ecx+eax*8+0x10] ; m_maps[index].map (stride 0x48, field +0x10)
//   ret 4
// __fastcall accessor: returns the map pointer at offset +0x10 of the
// index-th 0x48-byte MapEntry in this->m_maps (stored at this+0x20).
// Written as an explicit __fastcall(this, edx, index) so the compiler emits
// this=ecx, the index on the stack, and a `ret 4` frame-cleanup.
struct MapEntry {
    char pad0[0x10];              // 0x00..0x0F
    void* map;                    // 0x10  <- returned field
    char pad1[0x48 - 0x10 - 4];   // pad the entry to a 0x48-byte stride
};
struct CWorldMap {
    char pad[0x20];               // 0x00..0x1F
    MapEntry* m_maps;             // 0x20
};
void* __fastcall GetMap(CWorldMap* thisptr, void* /*edx*/, int index)
{
    return thisptr->m_maps[index].map;
}
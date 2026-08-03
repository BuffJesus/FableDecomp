// CEditWorldMap::GetEditMap(int index)
// this=ecx (__fastcall). Element stride = 12 bytes (3 dwords); returns dword at +4.
struct EditMapEntry {
    void* a;      // +0x0
    void* map;    // +0x4  <- returned
    void* c;      // +0x8
};

struct CEditWorldMap {
    char pad[0x3c];
    EditMapEntry* maps;   // +0x3c
};

void* __fastcall GetEditMap(CEditWorldMap* self, void* /*edx*/, int index)
{
    return self->maps[index].map;
}
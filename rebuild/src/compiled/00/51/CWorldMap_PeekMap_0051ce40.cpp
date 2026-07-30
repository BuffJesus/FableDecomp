struct MapEntry {
    char pad[0x10];
    void* ptr;      // +0x10
    char pad2[0x48 - 0x10 - 4];
};

struct CWorldMap {
    char pad[0x20];
    MapEntry* entries;  // +0x20
};

void* __fastcall PeekMap(CWorldMap* self, void* /*edx*/, int n) {
    return self->entries[n].ptr;
}
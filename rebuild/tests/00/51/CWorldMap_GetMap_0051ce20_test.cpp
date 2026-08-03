#include <cstdio>

// ---- function under test (identical body to source_cpp) ----
struct MapEntry {
    char pad0[0x10];
    void* map;
    char pad1[0x48 - 0x10 - 4];
};
struct CWorldMap {
    char pad[0x20];
    MapEntry* m_maps;
};
void* __fastcall GetMap(CWorldMap* thisptr, void* /*edx*/, int index)
{
    return thisptr->m_maps[index].map;
}

int main()
{
    // sizeof(MapEntry) must be exactly 0x48 for the lea+*8 scale to index right.
    if (sizeof(MapEntry) != 0x48) { printf("FAIL sizeof %u\n", (unsigned)sizeof(MapEntry)); return 1; }

    // Hand-built table of 4 entries; distinctive sentinel at each +0x10 field.
    MapEntry table[4];
    for (int i = 0; i < 4; ++i)
        table[i].map = (void*)(0xC0DE0000u + (unsigned)i);

    CWorldMap wm;
    *reinterpret_cast<MapEntry**>(reinterpret_cast<char*>(&wm) + 0x20) = table;

    CWorldMap* self = &wm;
    void* got = 0;
    for (int idx = 0; idx < 4; ++idx) {
        void* r = 0;
        // Retail convention: this in ecx, index pushed, callee does `ret 4`.
        __asm {
            mov  ecx, self
            mov  eax, idx
            push eax
            call GetMap
            mov  r, eax
        }
        void* want = (void*)(0xC0DE0000u + (unsigned)idx);
        if (r != want) { printf("FAIL idx=%d got=%p want=%p\n", idx, r, want); return 1; }
        if (idx == 2) got = r;
    }

    printf("OK_0x0051ce20 map[2]=%p\n", got);
    return 0;
}
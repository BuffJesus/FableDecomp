#include <cstdio>

struct MapEntry { char pad[0x24]; unsigned char used; char pad2[0x48-0x25]; };
struct CWorldMap { char pad[0x20]; MapEntry* maps; };
unsigned char __fastcall IsMapUsed(CWorldMap* self, int /*edx*/, int index)
{
    return self->maps[index].used;
}

int main()
{
    MapEntry entries[4];
    for (int i = 0; i < 4; ++i) entries[i].used = 0;
    entries[2].used = 1;
    entries[3].used = 0xAB;

    CWorldMap wm;
    wm.maps = entries;

    // __fastcall: this in ecx, dummy in edx, index on the stack.
    unsigned char r0 = IsMapUsed(&wm, 0, 0);
    unsigned char r2 = IsMapUsed(&wm, 0, 2);
    unsigned char r3 = IsMapUsed(&wm, 0, 3);

    if (r0 != 0 || r2 != 1 || r3 != 0xAB) {
        printf("FAIL r0=%u r2=%u r3=%u\n", r0, r2, r3);
        return 1;
    }
    // Confirm element stride is 0x48 by checking the flag offset directly.
    if ((int)((char*)&entries[1] - (char*)&entries[0]) != 0x48) {
        printf("FAIL stride=%d\n", (int)((char*)&entries[1]-(char*)&entries[0]));
        return 1;
    }
    printf("OK_0x0051ce00 r0=%u r2=%u r3=%u\n", r0, r2, r3);
    return 0;
}
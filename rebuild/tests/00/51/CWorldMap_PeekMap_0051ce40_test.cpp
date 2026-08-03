#include <stdio.h>
#include <string.h>

// ---- function under test (definition) ----
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

// ---- test harness ----
int main() {
    MapEntry arr[4];
    memset(arr, 0, sizeof(arr));
    arr[0].ptr = (void*)0xAAAA0000;
    arr[1].ptr = (void*)0xBBBB1111;
    arr[3].ptr = (void*)0xDDDD3333;

    CWorldMap wm;
    memset(&wm, 0, sizeof(wm));
    wm.entries = arr;

    // verify offset/stride assumptions
    if ((char*)&wm.entries - (char*)&wm != 0x20) { printf("BAD_OFFSET\n"); return 1; }
    if (sizeof(MapEntry) != 0x48) { printf("BAD_STRIDE %d\n", (int)sizeof(MapEntry)); return 1; }

    void* r1; void* r3;
    CWorldMap* p = &wm;
    // __fastcall via asm shim: ecx=this, one 4-byte arg pushed, callee cleans (ret 4)
    __asm {
        push 1
        mov  ecx, p
        call PeekMap
        mov  r1, eax
    }
    __asm {
        push 3
        mov  ecx, p
        call PeekMap
        mov  r3, eax
    }

    if (r1 != (void*)0xBBBB1111) { printf("FAIL r1=%p\n", r1); return 1; }
    if (r3 != (void*)0xDDDD3333) { printf("FAIL r3=%p\n", r3); return 1; }

    printf("OK_0x0051ce40\n");
    return 0;
}
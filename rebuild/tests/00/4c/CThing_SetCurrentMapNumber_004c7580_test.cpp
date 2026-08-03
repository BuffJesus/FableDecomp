#include <cstdio>
#include <cstring>

struct CThing { char pad[0x8e]; short currentMapNumber; short after; };

void __fastcall SetCurrentMapNumber(CThing *self, int edx, short mapNumber) {
    self->currentMapNumber = mapNumber;
}

int main() {
    CThing t;
    memset(&t, 0, sizeof(t));
    t.after = 0x1234;
    CThing *pt = &t;
    short arg = (short)0x5AA5;
    __asm {
        push arg
        mov ecx, pt
        call SetCurrentMapNumber
    }
    if (t.currentMapNumber != (short)0x5AA5) { printf("FAIL field=%d\n", t.currentMapNumber); return 1; }
    if (t.after != (short)0x1234) { printf("FAIL overwrote neighbour after=%d\n", t.after); return 1; }
    printf("OK_0x004c7580 map=%d\n", t.currentMapNumber);
    return 0;
}
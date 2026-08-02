#include <cstdio>
#include <cstring>

struct CThing { char pad[4]; short currentMapNumber_b; short after; };

void __fastcall SetCurrentMapNumber_b(CThing *self, int edx, short mapNumber) {
    self->currentMapNumber_b = mapNumber;
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
        call SetCurrentMapNumber_b
    }
    if (t.currentMapNumber_b != (short)0x5AA5) { printf("FAIL field=%d\n", t.currentMapNumber_b); return 1; }
    if (t.after != (short)0x1234) { printf("FAIL overwrote neighbour after=%d\n", t.after); return 1; }
    printf("OK_0x00c1bbd0 map=%d\n", t.currentMapNumber_b);
    return 0;
}
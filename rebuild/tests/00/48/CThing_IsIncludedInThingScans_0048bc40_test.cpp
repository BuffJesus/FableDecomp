#include <stdio.h>

struct CThing {
    char pad[0x91];
    unsigned char flags;
};

int __fastcall IsIncludedInThingScans(CThing *self)
{
    unsigned char f = self->flags;
    unsigned char nf = ~f;
    if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
        return 1;
    return 0;
}

static int call_it(CThing *t)
{
    int r;
    void *fn = (void*)&IsIncludedInThingScans;
    __asm {
        mov ecx, t
        call fn
        mov r, eax
    }
    return r;
}

int main()
{
    CThing t;
    // 0x04|0x08 = 0x0C -> should return 1
    t.flags = 0x0C;
    if (call_it(&t) != 1) { printf("FAIL flags=0x0C\n"); return 1; }
    // missing 0x04
    t.flags = 0x08;
    if (call_it(&t) != 0) { printf("FAIL flags=0x08\n"); return 1; }
    // missing 0x08
    t.flags = 0x04;
    if (call_it(&t) != 0) { printf("FAIL flags=0x04\n"); return 1; }
    // 0x0C but 0x01 also set -> 0
    t.flags = 0x0D;
    if (call_it(&t) != 0) { printf("FAIL flags=0x0D\n"); return 1; }
    // 0x0C but 0x20 also set -> 0
    t.flags = 0x2C;
    if (call_it(&t) != 0) { printf("FAIL flags=0x2C\n"); return 1; }
    // extra harmless bits (0x40|0x80|0x02) + 0x0C -> 1
    t.flags = 0xCE;
    if (call_it(&t) != 1) { printf("FAIL flags=0xCE\n"); return 1; }
    printf("OK_0x0048bc40\n");
    return 0;
}
#include <stdio.h>

struct CPlayer {
    char pad[0x20a];
    bool lockA;   // +0x20a
    bool lockB;   // +0x20b
    bool lockC;   // +0x20c
};

bool __fastcall IsTargetLocked(CPlayer *this_ptr)
{
    return this_ptr->lockA || this_ptr->lockB || this_ptr->lockC;
}

// __fastcall passes 'this' in ecx; use an explicit __asm shim to guarantee ecx = this.
static bool call_it(CPlayer *p)
{
    bool r;
    __asm {
        mov ecx, p
        call IsTargetLocked
        mov  r, al
    }
    return r;
}

int main()
{
    CPlayer p;
    for (int a = 0; a < 2; ++a)
    for (int b = 0; b < 2; ++b)
    for (int c = 0; c < 2; ++c) {
        p.lockA = (a != 0);
        p.lockB = (b != 0);
        p.lockC = (c != 0);
        bool expected = (a || b || c);
        bool got = call_it(&p);
        if (got != expected) {
            printf("FAIL a=%d b=%d c=%d expected=%d got=%d\n", a, b, c, (int)expected, (int)got);
            return 1;
        }
    }
    printf("OK_0x004874a0\n");
    return 0;
}
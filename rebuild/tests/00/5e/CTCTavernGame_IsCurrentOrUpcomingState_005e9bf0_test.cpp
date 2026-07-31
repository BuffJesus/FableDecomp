#include <stdio.h>

struct CTCTavernGame {
    char _pad[0xa8];
    int stateA;   /* +0xa8 */
    char _pad2[0xb0 - 0xa8 - 4];
    int stateB;   /* +0xb0 */
    char _pad3[0xc0 - 0xb0 - 4];
    int stateC;   /* +0xc0 */
};

static bool IsCurrentOrUpcomingState(int state)
{
    CTCTavernGame* p;
    __asm { mov p, ecx }
    if (p->stateC == state) return true;
    if (p->stateA == state) return true;
    if (p->stateB == state) return true;
    return false;
}

static bool call_it(CTCTavernGame* self, int state)
{
    bool r;
    __asm {
        push state
        mov  ecx, self
        call IsCurrentOrUpcomingState
        mov  r, al
    }
    return r;
}

int main()
{
    CTCTavernGame g;
    g.stateA = 10;
    g.stateB = 20;
    g.stateC = 30;

    /* matches on each field */
    if (!call_it(&g, 30)) { printf("FAIL stateC\n"); return 1; }
    if (!call_it(&g, 10)) { printf("FAIL stateA\n"); return 1; }
    if (!call_it(&g, 20)) { printf("FAIL stateB\n"); return 1; }
    /* no match */
    if (call_it(&g, 99)) { printf("FAIL nomatch\n"); return 1; }

    printf("OK_0x005e9bf0\n");
    return 0;
}
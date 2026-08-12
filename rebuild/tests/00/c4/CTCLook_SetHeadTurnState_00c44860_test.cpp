#include <stdio.h>

struct CTCLook {
    char pad0[0x14];
    unsigned long flags;   // +0x14
    char pad1[0x100 - 0x18];
    long headTurnState;    // +0x100
    void SetHeadTurnState(long state);
};

void CTCLook::SetHeadTurnState(long state)
{
    if (this->headTurnState != state) {
        this->headTurnState = state;
        this->flags |= 0x2000;
    }
}

int main()
{
    CTCLook o;
    o.flags = 0;
    o.headTurnState = 5;

    // changing value sets flag
    o.SetHeadTurnState(9);
    if (o.headTurnState != 9) { printf("FAIL1\n"); return 1; }
    if ((o.flags & 0x2000) == 0) { printf("FAIL2\n"); return 1; }

    // same value: no flag change
    o.flags = 0;
    o.SetHeadTurnState(9);
    if (o.flags != 0) { printf("FAIL3\n"); return 1; }

    // preexisting flag bits preserved via OR
    o.flags = 0x1;
    o.SetHeadTurnState(2);
    if (o.flags != (0x1 | 0x2000)) { printf("FAIL4\n"); return 1; }

    printf("HEADTURN_OK\n");
    return 0;
}
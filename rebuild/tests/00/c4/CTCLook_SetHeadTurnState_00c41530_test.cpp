#include <stdio.h>

struct CTCLook {
    char pad0[0x190];
    long headTurnState;
    char pad2[0x1d8 - 0x194];
    unsigned long flags;
};

void __fastcall SetHeadTurnState(CTCLook* self, int edx_dummy, long state)
{
    if (self->headTurnState != state) {
        self->headTurnState = state;
        self->flags |= 0x100000;
    }
}

int main()
{
    CTCLook o;
    o.headTurnState = 5;
    o.flags = 0x1;

    SetHeadTurnState(&o, 0, 5);
    if (o.headTurnState != 5) { printf("bad1\n"); return 1; }
    if (o.flags != 0x1) { printf("bad2\n"); return 1; }

    SetHeadTurnState(&o, 0, 9);
    if (o.headTurnState != 9) { printf("bad3\n"); return 1; }
    if (o.flags != (0x1 | 0x100000)) { printf("bad4\n"); return 1; }

    printf("BEHAVIOUR_OK\n");
    return 0;
}
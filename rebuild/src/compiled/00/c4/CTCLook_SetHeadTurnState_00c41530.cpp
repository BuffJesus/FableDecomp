struct CTCLook {
    char pad0[0x190];
    long headTurnState;   // +0x190
    char pad2[0x1d8 - 0x194];
    unsigned long flags;  // +0x1d8
};

void __fastcall SetHeadTurnState(CTCLook* self, int edx_dummy, long state)
{
    if (self->headTurnState != state) {
        self->headTurnState = state;
        self->flags |= 0x100000;
    }
}
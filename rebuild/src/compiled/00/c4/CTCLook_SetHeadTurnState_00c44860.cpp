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
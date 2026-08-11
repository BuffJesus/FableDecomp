// CBrainOutput::SetAsPumpCombatActions  @ 0x00c20460
// void __fastcall(CBrainOutput* this, int arg)

struct Member {
    // non-virtual __fastcall method taking one int arg (target 0xc1bc10)
    void DoPump(int arg);
};

struct CBrainOutput {
    unsigned int flags;   // +0x00
    int          pad4;    // +0x04
    Member       member;  // +0x08

    void SetAsPumpCombatActions(int arg);
};

void CBrainOutput::SetAsPumpCombatActions(int arg) {
    this->member.DoPump(arg);
    this->flags |= 0x80;
}
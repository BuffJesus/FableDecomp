// CBrainOutput::SetAsPumpCombatActions @ 0x00c20480
// __fastcall void(CBrainOutput* this, long a)

struct SubObj {
    void method(long a); // extern __fastcall, call target 0xc1bb80
};

struct CBrainOutput {
    unsigned long flags; // +0
    long pad4;           // +4
    SubObj sub;          // +8
    void SetAsPumpCombatActions(long a);
};

void CBrainOutput::SetAsPumpCombatActions(long a)
{
    this->sub.method(a);
    this->flags |= 4;
}
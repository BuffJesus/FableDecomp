// CBrainOutput::SetAsPumpCombatActions @ 00c204a0
struct SubObj {
    void call(long a);   // __fastcall member: this in ecx, long on stack
};

struct CBrainOutput {
    unsigned long flags;   // +0
    char pad[4];           // +4
    SubObj sub;            // +8
};

void __fastcall SetAsPumpCombatActions(CBrainOutput* self, int /*edx*/, long a) {
    self->sub.call(a);
    self->flags |= 8;
}
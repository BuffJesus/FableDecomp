// CBrainOutput::SetAsPumpCombatActions @ 00c203e0
// retail: mov eax,[esp+4]; push esi; mov esi,ecx; push eax;
//         lea ecx,[esi+8]; call 0xc1bb40; or dword[esi],2; pop esi; ret 4

struct SubObj {
    void Set(long a);  // __fastcall member: ecx=this, arg pushed
};

struct CBrainOutput {
    unsigned long flags; // +0
    long pad;            // +4
    SubObj sub;          // +8
};

void __fastcall SetAsPumpCombatActions(CBrainOutput* self, int /*edx*/, long a)
{
    self->sub.Set(a);
    self->flags |= 2;
}
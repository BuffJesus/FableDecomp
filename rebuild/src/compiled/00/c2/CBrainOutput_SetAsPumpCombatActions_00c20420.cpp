// CBrainOutput::SetAsPumpCombatActions @ 0x00c20420
// void __fastcall(CBrainOutput* self, int arg)

struct SubThing {
    void Apply(int arg);   // 0xc1bc70, __fastcall on self+8
};

struct CBrainOutput {
    unsigned int flags;    // +0x00
    int pad4;              // +0x04
    SubThing sub;          // +0x08
    void SetAsPumpCombatActions(int arg);
};

void CBrainOutput::SetAsPumpCombatActions(int arg)
{
    this->sub.Apply(arg);
    this->flags |= 0x200;
}
// CSpellContainerList::AddChild @ 0x006b9510
// Dual member-forwarder: __fastcall AddChild(this=ecx, arg on stack) calls a
// __fastcall setter (this=ecx, arg on stack) on subobjects at +4 and +0x10.
// Modeled as native C++ members so both are implicit __fastcall (stack arg).

struct CComp;

struct CountedPtr {
    void* p;
    void Assign(CComp* src);   // out-of-line __fastcall, one stack arg
};

struct CSpellContainerList {
    CountedPtr pad0;   // +0
    CountedPtr sub1;   // +4
    CountedPtr pad2;   // +8
    CountedPtr pad3;   // +0xC
    CountedPtr sub2;   // +0x10
    void AddChild(CComp* arg);
};

void CSpellContainerList::AddChild(CComp* arg)
{
    this->sub1.Assign(arg);
    this->sub2.Assign(arg);
}
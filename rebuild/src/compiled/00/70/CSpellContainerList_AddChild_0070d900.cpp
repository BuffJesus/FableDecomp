// CSpellContainerList::AddChild @ 0x0070d900
// Forwards the arg pointer to a member on the sublist at this+0x58 and this+0x64.

struct CComponentPtr { void* p; };

struct CSubList {
    char pad[4];
    // real member -> __fastcall: ecx=this, arg pushed on stack (matches retail 0x7118c0)
    void Push(CComponentPtr* c);
};

struct CSpellContainerList {
    char pad[0x58];
    CSubList listA;   // +0x58
    char pad2[0x64 - 0x58 - sizeof(CSubList)];
    CSubList listB;   // +0x64

    void AddChild(CComponentPtr* c);
};

void CSpellContainerList::AddChild(CComponentPtr* c)
{
    this->listA.Push(c);
    this->listB.Push(c);
}
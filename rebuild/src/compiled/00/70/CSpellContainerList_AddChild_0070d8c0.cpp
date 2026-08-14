// CSpellContainerList::AddChild @ 0x0070d8c0

struct CCountedPointer { void* p; };

struct SubList {
    char pad[4];
    void Insert(CCountedPointer* arg);  // __fastcall member; callee @ 0x711800
};

struct CSpellContainerList {
    char pad[0x58];
    SubList a;     // +0x58
    char gap[0x64 - 0x58 - sizeof(SubList)];
    SubList b;     // +0x64
    void AddChild(CCountedPointer* arg);
};

void CSpellContainerList::AddChild(CCountedPointer* arg)
{
    this->a.Insert(arg);
    this->b.Insert(arg);
}
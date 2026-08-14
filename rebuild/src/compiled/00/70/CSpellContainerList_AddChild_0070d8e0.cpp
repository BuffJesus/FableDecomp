// Byte-exact reconstruction of CSpellContainerList::AddChild @ 0x0070d8e0

struct CCountedPointer { void* p; };

// Subobject list type whose AddChild(CCountedPointer*) lives at 0x711860.
// Implicit member => VC7.1 __fastcall: this in ecx, arg pushed on stack.
struct CSubList {
    void AddChild(CCountedPointer* p);   // extern, resolves to 0x711860
};

struct CSpellContainerList {
    char pad[0x58];
    CSubList a;   // +0x58
    char pad2[0x64 - 0x58 - sizeof(CSubList)];
    CSubList b;   // +0x64
    void AddChild(CCountedPointer* ptr);
};

void CSpellContainerList::AddChild(CCountedPointer* ptr)
{
    this->a.AddChild(ptr);
    this->b.AddChild(ptr);
}
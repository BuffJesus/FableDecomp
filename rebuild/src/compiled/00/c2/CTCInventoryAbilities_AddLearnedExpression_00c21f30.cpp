// CTCInventoryAbilities::AddLearnedExpression @ 00c21f30
struct CTCBase;

// std::list<CTCBase*>; remove() is a real thiscall member (ecx=this, one stack
// arg by const-reference, callee-cleaned ret 4) matching callee @ 0xc22820.
struct TList {
    void* head;
    void remove(CTCBase* const& val);   // @ 0xc22820
};

struct CTCInventoryAbilities {
    char pad0[0x30];          // +0
    TList learned;            // +0x30
};

// retail is __fastcall: this in ECX, `a` is a `long` STACK arg (ret 4). Add a
// dummy EDX param to force `a` onto the stack. Passing the `long` as a
// `CTCBase*` (a conversion) materializes a temporary into the reused arg slot
// before its address is taken by-reference, reproducing retail's self-copy
// (mov eax,[esp+4] ... mov [esp+8],eax).
void __fastcall AddLearnedExpression(CTCInventoryAbilities* self, void* /*edx*/, long a)
{
    self->learned.remove((CTCBase*)a);
}
// CTCInventoryAbilities::AddLearnedExpression @ 006404b0
struct CTCBase;

// std::list<CTCBase*>; remove() is a real thiscall member (ecx=this, one stack
// arg by const-reference, callee-cleaned ret 4) matching callee @ 0x6405b0.
struct TList {
    void* head;
    void remove(CTCBase* const& val);   // @ 0x6405b0
};

struct CTCInventoryAbilities {
    int pad0;                 // +0
    TList learned;            // +4
};

// retail is __fastcall: this in ECX, `a` is a `long` STACK arg (ret 4). Under the
// harness __fastcall->__fastcall, so add a dummy EDX param to force `a` onto the
// stack. Passing the `long` as a `CTCBase*` (a conversion) materializes a
// temporary into the reused arg slot before its address is taken by-reference,
// reproducing retail's self-copy (mov eax,[esp+4] ... mov [esp+8],eax).
void __fastcall AddLearnedExpression(CTCInventoryAbilities* self, void* /*edx*/, long a)
{
    self->learned.remove((CTCBase*)a);
}
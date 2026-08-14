// Byte-exact reconstruction of _Cons_val<...pair<CCharString const,CArray<unsigned long> >...>
// Retail 0x007ae0c0. this=ecx=dest pair storage, edx=src pair*.
// Copy-construct first (CCharString @+0) then second (CArray @+4) from *val.
// Callees are __fastcall member copy-ctors (this in ecx, src pushed on stack) -> no edx setup.

struct CCharString {
    char* p;
    // copy-ctor modelled as a member that takes src on the stack (this in ecx)
    void CopyFrom(CCharString* src);
};

struct CArrayUL {
    void* a; void* b; void* c;
    void CopyFrom(CArrayUL* src);
};

struct Pair {
    CCharString first;   // +0
    CArrayUL    second;  // +4
};

void __fastcall Cons_val(Pair* self, Pair* val)
{
    if (self)
    {
        self->first.CopyFrom(&val->first);
        self->second.CopyFrom(&val->second);
    }
}
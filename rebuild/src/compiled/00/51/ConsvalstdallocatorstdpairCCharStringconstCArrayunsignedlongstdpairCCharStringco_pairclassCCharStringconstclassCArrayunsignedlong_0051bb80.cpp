// _Cons_val for std::allocator<std::pair<CCharString const,CArray<unsigned long> > >
// Copy-constructs a pair at *self from source *src.
// Retail 0x0051bb80: __fastcall(self in ecx, src in edx)

// CCharString: this-call copy-ctor helper (0x514eb0) -> only 'this' in ecx, src on stack
struct CCharString {
    char pad[0x0c];
    void CopyFrom(void* src);   // thiscall: this in ecx, src pushed
};
// CArray<unsigned long>: this-call copy-ctor helper (0x51b4c0)
struct CArrayUL {
    char pad[0x0c];
    void CopyFrom(void* src);   // thiscall
};

struct PairKV {
    CCharString first;   // 0x00
    CArrayUL    second;  // 0x0c
};

void __fastcall Cons_val_pair(PairKV* self, PairKV* src)
{
    if (self) {
        self->first.CopyFrom(&src->first);
        self->second.CopyFrom(&src->second);
    }
}
// Byte-exact reconstruction of _Cons_val for pair<CCharString const, CArray<unsigned long> >
// retail 0x00518b60

struct CCharString {
    char* p;
    void copy(const CCharString* src);  // __fastcall copy-construct
};
struct CArrayUL {
    void* vt; unsigned long* a; int n; int c;
    void copy(const CArrayUL* src);      // __fastcall copy-construct
};

struct PairT {
    CCharString first;   // +0
    CArrayUL    second;  // +4
};

// self = ecx (dest pair), src = edx (source pair)
void __fastcall Cons_val_pair(PairT* self, PairT* src)
{
    if (self) {
        self->first.copy(&src->first);
        self->second.copy(&src->second);
    }
}
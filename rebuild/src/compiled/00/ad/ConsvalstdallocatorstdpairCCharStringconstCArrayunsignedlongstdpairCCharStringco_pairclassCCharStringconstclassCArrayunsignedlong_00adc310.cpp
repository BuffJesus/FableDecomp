// _Cons_val for std::pair<CCharString const, CArray<unsigned long> >
// Copy-constructs the pair members at *self from *src.

struct CCharString {
    char* p;        // +0
    int   len;      // +4
    int   cap;      // +8  -> sizeof 0xc, second member lands at +0xc
    void ctor(CCharString* src);   // thiscall copy-ctor @0x535470
};

struct CArrayUL {
    void* a;        // +0
    void* b;        // +4
    unsigned long n;// +8
    void ctor(CArrayUL* src);      // thiscall copy-ctor @0xa96e40
};

struct Pair {
    CCharString first;   // +0
    CArrayUL    second;  // +0xc
};

// _Cons_val: __fastcall(self=ecx, src=edx)
void __fastcall Cons_val(Pair* self, Pair* src)
{
    if (self) {
        self->first.ctor(&src->first);
        self->second.ctor(&src->second);
    }
}
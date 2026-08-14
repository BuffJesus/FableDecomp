// Byte-exact reconstruction of _Cons_val<...pair<CCharString const,std::pair<long,long> >...>
// Retail 0x00a48650. this=ecx=dest pair storage, edx=src pair*.
// Copy-construct first (CCharString @+0) via member copy-ctor; second (pair<long,long> @+4,+8) inline copy.

struct CCharString {
    char* p;
    void CopyFrom(CCharString* src);
};

struct PairLL {
    long a;  // +0 (at offset +4 of outer)
    long b;  // +4 (at offset +8 of outer)
};

struct Pair {
    CCharString first;   // +0
    PairLL      second;  // +4
};

void __fastcall Cons_val(Pair* self, Pair* val)
{
    if (self)
    {
        self->first.CopyFrom(&val->first);
        self->second.a = val->second.a;
        self->second.b = val->second.b;
    }
}
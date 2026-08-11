// _Cons_val for std::allocator<std::pair<CWideString const,float> >
// __fastcall(dest, src): ecx=dest pair (esi), edx=src pair (edi).
// If dest!=0: construct dest->first (thiscall this=&dest->first=dest, arg=src),
// then copy float second [esi+0xc] = [edi+0xc]. Allocator arg is stateless -> elided.

struct PairSrc;

struct CWideString {
    void* buf;   // +0x0
    int   a;     // +0x4
    int   b;     // +0x8
    void CopyCtor(PairSrc* src);   // thiscall -> retail sub 0x4f9408
};

struct PairSrc {          // std::pair<CWideString,float>
    CWideString first;    // +0x0
    float       second;   // +0xc
};

struct PairDst {          // std::pair<CWideString const,float>
    CWideString first;    // +0x0
    float       second;   // +0xc
};

void __fastcall _Cons_val(PairDst* dest, PairSrc* src)
{
    if (dest) {
        dest->first.CopyCtor(src);
        dest->second = src->second;
    }
}
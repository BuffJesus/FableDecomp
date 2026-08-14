// Byte-exact reconstruction of STL _Cons_val for
// pair<CWideString const, CChecksumCacheFile::CEntry>
// retail 0x00a6ed60
#include <new>

// CWideString: single pointer-sized member; copy ctor is an out-of-line thiscall
struct CWideString {
    wchar_t* p;
    CWideString(const CWideString& o);   // extern copy ctor @ 0x99ec30
};

struct CEntry {
    int raw[13];   // 52 bytes, copied verbatim
};

struct MyPair {
    CWideString first;   // +0
    CEntry     second;   // +4
};

// dest -> ecx, src -> edx (fastcall); no stack args -> plain ret
void __fastcall Cons_val_CEntry___(MyPair* dest, MyPair* src)
{
    if (dest) {
        new (&dest->first) CWideString(src->first);
        dest->second = src->second;
    }
}
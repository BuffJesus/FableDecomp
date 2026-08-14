// Byte-exact reconstruction of _Cons_val at 0x004b8330
// std::allocator<std::pair<CCharString const, std::pair<long,long> > >::_Cons_val
// Copy-constructs pair<CCharString const, pair<long,long> > at *dst from *src.

inline void* operator new(unsigned int, void* p) { return p; }

struct CCharString {
    char* p;                       // +0
    CCharString(const CCharString&); // copy ctor -> retail 0x0099b720 (external)
};

struct LongPair { long first; long second; };

// pair<CCharString const, pair<long,long> >
struct MyPair {
    CCharString first;   // +0
    LongPair    second;  // +4, +8
};

// _Cons_val(allocator* [ecx unused], pair* dst, pair* src)
// Retail codegen: ecx=dst(self), edx=src. Model as __fastcall(dst, src).
void __fastcall pair_long_long_____Cons_val(MyPair* dst, MyPair* src)
{
    if (dst) {
        // placement copy-construct the CCharString member (thiscall: ecx=dst, push src)
        new (dst) CCharString(src->first);
        dst->second.first  = src->second.first;
        dst->second.second = src->second.second;
    }
}
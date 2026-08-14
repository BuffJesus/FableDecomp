// Byte-exact reconstruction of _Cons_val for
//   pair<CCharString const, pair<long,long> >
// at retail 0x00511f30.
//
// Disasm shows: ecx = dest pair*, edx = src pair*.
// It calls CCharString copy-ctor (this=dest string subobject, arg=src string
// subobject) then copies the two longs [+4],[+8].
//
// Model CCharString as a class with a copy ctor at the engine address; give the
// pair the layout: [0..3]=CCharString(one ptr), [4]=long first, [8]=long second.

// placement-new declaration (must precede use)
inline void* operator new(unsigned int, void* p) { return p; }

struct CCharString {
    void* p;
    // copy ctor -> engine 0x0099ec30 (declared, not defined; matched as call)
    CCharString(const CCharString& o);
};

// The second member is std::pair<long,long>
struct LongPair {
    long first;
    long second;
};

// The value_type is std::pair<CCharString const, std::pair<long,long> >
struct ValPair {
    CCharString first;   // offset 0 (1 ptr)
    LongPair   second;   // offset 4,8
};

// allocator is empty
struct Alloc {};

// _Cons_val(allocator&, value_type* dest, value_type* src)
// __fastcall: arg1(Alloc*)=ecx, arg2(dest)=edx ... but disasm has dest=ecx.
// MSVC actually generates this as a member of allocator-ish; the real codegen
// puts dest in ecx. We model it as a __fastcall free fn where the FIRST pointer
// param lands in ecx = dest, SECOND (src) in edx.
extern "C" void __fastcall Cons_val(ValPair* dest, ValPair* src)
{
    if (dest) {
        // placement copy-construct the CCharString first member
        new (&dest->first) CCharString(src->first);
        dest->second.first  = src->second.first;
        dest->second.second = src->second.second;
    }
}
// _Cons_val for allocator<pair<CCharString const,bool>>
// dest pair* in ecx, src pair* in edx.
// pair layout: CCharString first at +0, bool second at +0xd.

#include <new>

struct CCharString {
    char pad[13];
    CCharString(const CCharString& o);  // copy-ctor at 0x673e10
};

struct MyPair {
    CCharString first;   // +0 .. +0xc
    bool second;         // +0xd
};

// self(ecx)=_Ptr(dest), edx=_Val(src)
void __fastcall _Cons_val(MyPair* _Ptr, MyPair* _Val)
{
    if (_Ptr)
    {
        ::new (static_cast<void*>(&_Ptr->first)) CCharString(_Val->first);
        _Ptr->second = _Val->second;
    }
}
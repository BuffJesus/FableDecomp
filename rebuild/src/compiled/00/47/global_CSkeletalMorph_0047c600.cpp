// std::_Copy_impl<CTCSkeletalMorph::CSkeletalMorph*, CTCSkeletalMorph::CSkeletalMorph*> @ 0x0047c600
//
// __fastcall, returns CSkeletalMorph* (advanced _Dest), ret 0xc (3 stack dwords):
//   ecx = _First, edx = _Last, [stack] _Dest, _Dest2, _Nonscalar tag.
// Copies [_First,_Last) forward into _Dest.
//
// Retail codegen shape:
//   sub edx,esi ; imul 0x2aaaaaab ; sar 2 ; +sign  => signed COUNT = (_Last-_First)/0x18
//   test eax,eax ; jle                              => `0 < N` signed entry guard
//   ... ; add esi,0x18 ; add edi,0x18 ; dec ebx ; jne  => counted countdown loop
// The `for (int N = (int)(_Last-_First); 0 < N; --N)` spelling (as in the
// byte-exact _Copy_backward @ 0x00485372 sibling) is what forces the signed
// magic-divide + `test;jle` + `dec;jne` shape instead of a `_First != _Last`
// pointer-compare (which would enregister differently and diverge).
//
// Each 0x18-byte (24) element is `{ u32 a; MemberA b; u32 c; MemberB d; }`:
//   [+0x00] u32      copied inline (mov eax,[esi]; mov [edi],eax)
//   [+0x04] MemberA  4 bytes, out-of-line __fastcall operator= (call A)
//   [+0x08] u32      copied inline (mov edx,[esi+8]; mov [edi+8],edx)
//   [+0x0c] MemberB  12 bytes, out-of-line __fastcall operator= (call B)
// The whole-element `*_Dest = *_First` lowers to the compiler-generated
// CSkeletalMorph::operator= which copies the two u32 members inline and calls
// the two (distinct, relocation-masked) member operator= helpers.

struct MemberA {                 // sizeof == 4
    unsigned long _p;
    MemberA& operator=(const MemberA& _rhs);   // out-of-line -> __fastcall call A
};

struct MemberB {                 // sizeof == 12
    unsigned long _p0, _p1, _p2;
    MemberB& operator=(const MemberB& _rhs);   // out-of-line -> __fastcall call B
};

struct CSkeletalMorph {          // sizeof == 0x18 (24)
    unsigned long _a;            // +0x00
    MemberA       _b;            // +0x04
    unsigned long _c;            // +0x08
    MemberB       _d;            // +0x0c
    // implicit copy-assignment: _a, _b (call A), _c, _d (call B)
};

struct _Nonscalar_ptr_iterator_tag {};   // empty tag, one stack slot

CSkeletalMorph* __fastcall Copy_impl_CSkeletalMorph_0047c600(
    CSkeletalMorph* _First,
    CSkeletalMorph* _Last,
    CSkeletalMorph* _Dest,
    CSkeletalMorph* _Dest2,
    _Nonscalar_ptr_iterator_tag)
{   // copy [_First, _Last) forward into _Dest
    (void)_Dest2;
    for (int _N = (int)(_Last - _First); 0 < _N; --_N) {
        *_Dest = *_First;
        ++_First;
        ++_Dest;
    }
    return (_Dest);
}
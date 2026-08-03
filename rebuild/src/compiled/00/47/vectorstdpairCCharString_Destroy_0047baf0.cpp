// Genuine-C++ reconstruction of retail 0x0047baf0
// std::_Destroy<...>(_First, _Last, _Al)  over a vector<std::pair<CCharString,...>>
//
// The element is a 12-byte (stride 0x0c) aggregate of THREE CCharString
// sub-objects. Retail destroys the range [_First,_Last) one element at a time,
// destroying each element's three CCharString members in REVERSE declaration
// order via three INLINE direct calls to the same non-virtual
// CCharString::~CCharString (all masked to 0x51fa20):
//   lea ecx,[esi+0x8]; call ~CCharString   -> member +0x08
//   lea ecx,[esi+0x4]; call ~CCharString   -> member +0x04
//   mov ecx,esi;       call ~CCharString   -> member +0x00
//   add esi,0x0c                            -> ++Elem stride 0x0c
// The call rel32 is relocation-masked in parity.
//
// Loop shape: the range is walked with a pointer `!=` compare, so VC7.1 emits a
// top entry guard `cmp esi,edi; je end` (retail byte 0x8 = 0x74 = je, the
// signed/pointer form) plus a bottom back-edge `cmp esi,edi; jne loop` (byte
// 0x2c = 0x75). This is a top-guarded do/while identical in structure to the
// landed byte-matching sibling _Uninit_copy_0047b670 (same pointer `!=` guard,
// same 12-byte triple-sub-object element, same out-of-line member call forced by
// declaring-but-not-defining the leaf), differing only in that _Destroy runs the
// three member calls in REVERSE order (destructor semantics) and pops one stack
// arg (the allocator) via `ret 4` instead of `ret 8`.
//
// The favor-speed ('t') sweep aligns the loop body to 16 bytes, for which the
// assembler pads the gap after the top guard with a 6-byte `lea ebx,[ebx+0x0]`
// NOP (retail byte 0xa). No #pragma optimize here so the lander's s/t/g sweep
// can select the 't' form that reproduces that aligned, dual-guard shape.
//
// CCharString: a 4-byte handle whose destructor is DECLARED but not defined in
// this TU, so cl emits a direct (relocation-masked) call to it rather than
// inlining a body. Three explicit member destructor calls force the three
// separate inline calls (rather than one out-of-line synthesized
// Elem::~Elem).

class CCharString {
public:
    ~CCharString();            // out-of-line -> masked call 0x51fa20
private:
    void* _storage;            // 4-byte handle
};

struct Elem {
    CCharString _a;   // +0x00
    CCharString _b;   // +0x04
    CCharString _c;   // +0x08
};                    // sizeof == 0x0c

// __fastcall: ecx = _First, edx = _Last; one stack dword (the allocator) popped
// by `ret 4`.
extern "C" void __fastcall
Destroy_pair_CCharString_0047baf0(Elem* _First, Elem* _Last, void* /*_Al*/)
{
    for (; _First != _Last; ++_First)
    {
        _First->_c.~CCharString();   // +0x08
        _First->_b.~CCharString();   // +0x04
        _First->_a.~CCharString();   // +0x00
    }
}
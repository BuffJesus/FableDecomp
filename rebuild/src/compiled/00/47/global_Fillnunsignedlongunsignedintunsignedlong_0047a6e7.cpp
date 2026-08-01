#pragma optimize("s",on)
// Genuine-C++ reconstruction of retail 0x0047a6e7
// _Fill_n<unsigned_long*, unsigned_int, unsigned_long>  (leading real function)
//
// The manifest slice (86 bytes) OVER-CAPTURES. The real _Fill_n is only the
// first 0x26 bytes (offsets 0x00-0x25, ending `ret 4`). It is followed by 3
// int3 alignment fillers (0x26-0x28) and then a SEPARATE, unrelated function
// (0x29-0x55, a 3-member ctor ending `ret 0xc`). We reconstruct ONLY the
// leading _Fill_n; real_len = 0x26 = 38.
//
// This is the same MSVC7.1 std::_Fill_n idiom already landed byte-exact for the
// sibling 0x0047b630 (see rebuild/src/compiled/00/47/
// global_Fillnunsignedlongunsignedintunsignedlong_0047b630.cpp), differing only
// in the element type: here sizeof(Elem)==0x50 (stride `add esi,0x50`) and the
// fill value is a plain `unsigned long` scalar passed by value, assigned into
// each element through an out-of-line member operator= (an out-of-line
// __fastcall call: `mov ecx,element; push value; call`).
//
// Retail body (offsets 0x00-0x25):
//   this/dest in ecx (esi), count in edx (edi), value = one stack arg [esp+4].
//   entry guard: `test edx,edx; jbe done`  <= `count > 0` on UNSIGNED count
//     (byte 0x5 is 76=jbe, exactly what `> 0` unsigned emits; `!= 0` would give
//     74=je). Inner `if (dest)` => `test esi,esi; je skip`. Per element, if the
//   element ptr is non-null, invoke Elem::operator=(value) (out-of-line member
//   => __fastcall: push value, ecx=element, call). Advance by sizeof(Elem)=0x50.
//   Return the one-past-the-end destination pointer in eax; `ret 4` (one arg).
//
// The call target (rel32) is relocation-masked in parity, so operator= is left
// out-of-line (declared, not defined) to force the __fastcall call instruction.

struct Elem {
    // Out-of-line member operator= taking the scalar fill value. Being
    // out-of-line forces a __fastcall call (this in ecx, value on the stack),
    // matching retail `push [esp+0xc]; mov ecx,esi; call <masked>`.
    Elem& operator=(unsigned long _v);
    char _storage[0x50];   // sizeof(Elem) == 0x50 -> stride `add esi,0x50`
};

// __fastcall: dest in ecx, count in edx; the fill value is the single stack
// argument (ret 4). Returns the one-past-the-end destination pointer (esi).
extern "C" Elem* __fastcall
Fill_n_0047a6e7(Elem* _dest, unsigned int _count, unsigned long _value)
{
    // `_count > 0` (unsigned) yields the retail entry guard `test edx,edx; jbe`
    // (byte 0x5 == 76=jbe), not the `74=je` that `_count != 0` would emit.
    for (; _count > 0; --_count)
    {
        if (_dest)
            *_dest = _value;   // Elem::operator=(unsigned long) -> __fastcall call
        ++_dest;               // += sizeof(Elem) == 0x50
    }
    return _dest;
}
#pragma optimize("s",on)
// Genuine-C++ reconstruction of retail 0x00478be4:
// CArray<std::pair<unsigned_long,...> >::push_back -- the manifest labels it
// "push_back" but the body is the classic std vector-style
// resize(_Newsize, const T&) dispatch, element stride 0x48 == 72 bytes.
//
// this (ecx) is a {first,last} range header:
//   [ecx+0x00] = _First  (begin pointer)
//   [ecx+0x04] = _Last   (end pointer)
// stack args after the 3 register saves: _Newsize @ [esp+0x10], &_Val @ [esp+0x14].
//
// count() = (_Last - _First) / 0x48  is a signed pointer-difference division.
// Because 72 is not a power of two, VC7.1 under /O2 would strength-reduce the
// divide to a reciprocal magic-multiply (imul 0x38e38e39 + shifts).  Retail
// instead emits a real `push 0x48; cdq; pop reg; idiv reg`, which VC7.1 produces
// only when size-optimising the divide -- hence the file-scope
// #pragma optimize("s",on) (the same technique the landed genuine sibling
// 0x00475e92 uses).  That makes the compiler materialise the 0x48 divisor into a
// register via push-imm8/pop and do a true signed idiv, matching the retail bytes.
//
// Dispatch:
//   _Newsize <  size() : erase(begin() + _Newsize, end())           [callee A]
//   _Newsize >= size() : _Insert_n(end(), _Newsize - size(), _Val)  [callee B]
// erase / _Insert_n are members of the same container (this in ecx), declared
// but not defined here, so cl emits the direct relocation-masked calls with ecx
// preserved as `this`.  The slot address begin()+_Newsize on a 72==9*8 stride is
// built with the two-LEA chain (lea [esi+esi*8] = *9 ; lea [edi+eax*8] = first +
// idx*72), exactly as retail.

struct Elem { char _pad[0x48]; };   // sizeof == 72

struct CArrayElem {
    Elem* _First;   // +0x00
    Elem* _Last;    // +0x04

    Elem* begin() const { return _First; }
    Elem* end()   const { return _Last; }
    unsigned int size() const { return (unsigned int)(_Last - _First); }

    // Out-of-line members -> direct __fastcall calls, ecx = this (kept).
    void erase(Elem* _F, Elem* _L);
    void _Insert_n(Elem* _Where, unsigned int _Count, const Elem& _Val);

    void resize(unsigned int _Newsize, const Elem& _Val);
};

void CArrayElem::resize(unsigned int _Newsize, const Elem& _Val)
{
    if (_Newsize < size())
        erase(begin() + _Newsize, end());
    else
        _Insert_n(end(), _Newsize - size(), _Val);
}
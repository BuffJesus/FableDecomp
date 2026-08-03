#pragma optimize("s",on)
// Genuine C++ decompilation of retail 0x00477dea
// CArray<std::pair<unsigned_long, ...> >::push_back  (manifest label); the body
// is the classic MSVC vector resize(_Newsize, const T&) dispatch, identical in
// shape to the byte-matched sibling 0x00475e92 -- only the element STRIDE differs
// (0x88 = 136 bytes here vs 8 there), which forces a real signed idiv instead of
// a sar for size().
//
// Retail disasm mapped to source:
//   mov eax,[ecx+4]        ; _Last   (this+0x04)
//   push ebx
//   mov ebx,[ecx]          ; _First  (this+0x00)  -> held in ebx across branch
//   push esi; push edi
//   sub eax,ebx            ; _Last - _First  (byte span)
//   mov edi,0x88           ; stride constant
//   cdq                    ; sign-extend for idiv
//   mov esi,edi            ; scratch divisor = stride
//   idiv esi               ; eax = size() = (_Last-_First)/0x88
//   mov esi,[esp+0x10]     ; _Newsize (stack arg 1)
//   cmp esi,eax; jae INSERT_N ; _Newsize < size() -> ERASE, else INSERT_N
// ERASE (index < size):
//   push [ecx+4]           ; end()
//   imul esi,esi,0x88      ; _Newsize*0x88
//   add esi,ebx            ; begin() + _Newsize
//   push esi
//   call erase(begin()+_Newsize, end())     ; this in ecx (kept)
//   jmp END
// INSERT_N (index >= size):
//   mov eax,[ecx+4]; sub eax,ebx; cdq; idiv edi ; size() recomputed (edi still 0x88)
//   push [esp+0x14]        ; &_Val (const T& reference, one dword)
//   sub esi,eax            ; _Newsize - size()
//   push esi
//   push [ecx+4]           ; end()
//   call _Insert_n(end(), _Newsize-size(), _Val)  ; this in ecx (kept)
// END:
//   pop edi; pop esi; pop ebx; ret 8
//
// The stride 0x88 is materialized as `mov edi,0x88` then copied to esi for the
// first divisor because cl keeps the constant live in edi to reuse as the second
// idiv divisor on the insert path (no reload of `first`).  `_First` stays in the
// callee-saved ebx because it is read in BOTH branches; `_Newsize` stays in esi
// (also callee-saved) for the same reason.  erase / _Insert_n are out-of-line
// members of the same container (this in ecx) so cl emits direct,
// relocation-masked calls with ecx preserved as `this`; declared but not defined
// here.  _Val is passed by const reference (single dword pushed; ret 8).

struct Pair {
    // std::pair<unsigned long, ...>; whole element is 0x88 bytes wide.
    unsigned long first;   // +0x00
    unsigned char rest[0x84]; // +0x04 .. +0x87  (payload, opaque here)
};                          // sizeof == 0x88

struct CArrayPair {
    Pair* _First;  // +0x00
    Pair* _Last;   // +0x04

    Pair* begin() const { return _First; }
    Pair* end() const   { return _Last; }
    unsigned int size() const { return (unsigned int)(_Last - _First); }

    // Out-of-line members -> direct __fastcall calls, ecx = this (masked).
    void erase(Pair* _F, Pair* _L);
    void _Insert_n(Pair* _Where, unsigned int _Count, const Pair& _Val);

    void push_back(unsigned int _Newsize, const Pair& _Val);
};

void CArrayPair::push_back(unsigned int _Newsize, const Pair& _Val)
{
    if (_Newsize < size())
        erase(begin() + _Newsize, end());
    else
        _Insert_n(end(), _Newsize - size(), _Val);
}
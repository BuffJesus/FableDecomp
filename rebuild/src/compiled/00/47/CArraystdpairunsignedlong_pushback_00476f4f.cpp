#pragma optimize("s",on)
// Genuine C++ reconstruction of retail 0x00476f4f
// CArray<std::pair<unsigned_long, ...> >::resize  (manifest labels it
// "push_back"; the body is the classic resize(_Newsize, const T&) dispatch,
// identical in shape to the byte-matched sibling 0x00475e92 -- only the element
// STRIDE differs: here sizeof(T) == 0xc4 (196) instead of 8).
//
// Retail disasm:
//   mov eax,[ecx+4]          ; _Last   (this+0x04)
//   push ebx
//   mov ebx,[ecx]            ; _First  (this+0x00)
//   push esi
//   sub eax,ebx              ; _Last - _First (byte span)
//   push edi
//   mov edi,0c4h             ; stride constant materialized in a register
//   cdq
//   mov esi,edi              ; divisor mirrored into esi
//   idiv esi                 ; size() = (_Last-_First)/0xc4  (signed ptrdiff)
//   mov esi,[esp+0x10]       ; _Newsize (stack arg 1, after 3 saves)
//   cmp esi,eax
//   jae INSERT               ; _Newsize >= size() -> INSERT, else ERASE
// ERASE (_Newsize < size()):
//   push [ecx+4]             ; end()
//   imul esi,esi,0c4h        ; _Newsize * stride
//   add esi,ebx              ; _First + _Newsize*stride = begin()+_Newsize
//   push esi
//   call erase(begin()+_Newsize, end())          ; this in ecx (kept)
//   jmp END
// INSERT (_Newsize >= size()):
//   mov eax,[ecx+4]          ; _Last reloaded (end())
//   sub eax,ebx              ; _Last - _First
//   cdq
//   idiv edi                 ; /0xc4 (stride still resident in edi) = size()
//   push [esp+0x14]          ; &_Val (const T& reference, one dword)
//   sub esi,eax              ; _Newsize - size()
//   push esi
//   push [ecx+4]             ; end()
//   call _Insert_n(end(), _Newsize-size(), _Val)  ; this in ecx (kept)
// END:
//   pop edi; pop esi; pop ebx; ret 8
//
// Because the element stride (0xc4) is not a power of two, cl cannot fold the
// pointer-difference divide to a shift; it materializes the sizeof constant in
// a register and emits a real signed idiv (reusing edi across both computes).
// erase / _Insert_n are members of the same container (this in ecx), declared
// but not defined so cl emits the direct relocation-masked calls with ecx
// preserved as `this`.

struct Pair196 {
    unsigned long first;   // +0x00
    unsigned long pad[48]; // +0x04 .. +0xc0  (second member, 192 bytes)
};                         // sizeof == 0xc4 (196)

struct CArrayPair196 {
    Pair196* _First;  // +0x00
    Pair196* _Last;   // +0x04

    Pair196* begin() const { return _First; }
    Pair196* end() const   { return _Last; }
    unsigned int size() const { return (unsigned int)(_Last - _First); }

    // Out-of-line members -> direct __fastcall calls, ecx = this.
    void erase(Pair196* _F, Pair196* _L);
    void _Insert_n(Pair196* _Where, unsigned int _Count, const Pair196& _Val);

    void resize(unsigned int _Newsize, const Pair196& _Val);
};

void CArrayPair196::resize(unsigned int _Newsize, const Pair196& _Val)
{
    if (_Newsize < size())
        erase(begin() + _Newsize, end());
    else
        _Insert_n(end(), _Newsize - size(), _Val);
}
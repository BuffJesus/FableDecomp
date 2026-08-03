#pragma optimize("s",on)
// Byte-exact reconstruction of retail 0x0047662a
// CArray<std::pair<unsigned_long,...> >::resize  (manifest labels it "push_back";
// the body is the classic resize(_Newsize, const T&) dispatch -- identical in
// shape to the already-byte-matching sibling 0x00475e92, only the element size
// differs: this container's element is 0x5c (92) bytes, so the "size()" pointer
// difference is a signed idiv by 0x5c instead of the sibling's sar-by-3).
//
// Retail disasm:
//   push ebx
//   mov  ebx,[ecx+4]         ; _Last  (end())         this+0x04
//   push esi; push edi
//   mov  edi,[ecx]           ; _First (begin())       this+0x00
//   mov  eax,ebx; sub eax,edi ; _Last - _First (bytes)
//   push 5Ch; cdq; pop esi; idiv esi   ; size() = (_Last-_First)/0x5c  (signed)
//   mov  esi,[esp+0x10]      ; _Newsize (stack arg 1)
//   cmp  esi,eax; jae INSERT ; _Newsize >= size() -> _Insert_n, else erase
// ERASE (fall-through, _Newsize < size()):
//   imul esi,esi,5Ch         ; _Newsize * 0x5c
//   push ebx                 ; end()
//   add  esi,edi             ; begin() + _Newsize
//   push esi
//   call erase(begin()+_Newsize, end())        ; this in ecx (kept)
//   jmp  END
// INSERT (_Newsize >= size()):
//   mov  ebx,[ecx+4]         ; _Last reloaded (end())
//   push [esp+0x14]          ; &_Val (const T&, one dword)
//   mov  eax,ebx; sub eax,edi
//   push 5Ch; pop edi; cdq; idiv edi           ; size() recomputed
//   sub  esi,eax             ; _Newsize - size()
//   push esi
//   push ebx                 ; end()
//   call _Insert_n(end(), _Newsize-size(), _Val)   ; this in ecx (kept)
// END:
//   pop edi; pop esi; pop ebx; ret 8
//
// _Val is passed by const reference (single dword pushed; ret 8).  erase and
// _Insert_n are members of the same container (this in ecx), declared but not
// defined here so cl emits the direct relocation-masked calls with ecx preserved
// as `this`.  Both branches call size() freshly, reproducing retail's reload of
// _Last (end()) inside the insert arm.

struct Pair {
    unsigned long first;   // +0x00
    char          second[0x58]; // +0x04 .. 0x5b  -> sizeof(Pair) == 0x5c (92)
};

struct CArrayPair {
    Pair* _First;  // +0x00
    Pair* _Last;   // +0x04

    Pair* begin() const { return _First; }
    Pair* end() const   { return _Last; }
    unsigned int size() const { return (unsigned int)(_Last - _First); }

    // Out-of-line members -> direct __fastcall (__fastcall) calls, ecx = this.
    void erase(Pair* _F, Pair* _L);
    void _Insert_n(Pair* _Where, unsigned int _Count, const Pair& _Val);

    void resize(unsigned int _Newsize, const Pair& _Val);
};

void CArrayPair::resize(unsigned int _Newsize, const Pair& _Val)
{
    if (_Newsize < size())
        erase(begin() + _Newsize, end());
    else
        _Insert_n(end(), _Newsize - size(), _Val);
}
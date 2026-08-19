#pragma optimize("s",on)
// Byte-exact reconstruction of retail 0x004799c6
// CArray<std::pair<unsigned_long,...> >::resize  (manifest labels it "push_back";
// the body is the classic resize(_Newsize, const T&) dispatch, identical in
// shape to the byte-matched sibling 0x00475e92 but over an 0x54-byte element).
//
// Retail disasm:
//   mov ebx,[ecx+4]       ; _Last  (this+0x04)   loaded first
//   mov edi,[ecx]         ; _First (this+0x00)
//   mov eax,ebx; sub eax,edi          ; _Last - _First  (byte span)
//   push 0x54; pop esi; cdq; idiv esi ; size() = span / 0x54  (signed idiv)
//   mov esi,[esp+0x10]    ; _Newsize (stack arg 0)
//   cmp esi,eax; jae INSERT           ; _Newsize < size() -> ERASE, else INSERT
// ERASE (index < count):
//   imul esi,esi,0x54; add esi,edi    ; begin() + _Newsize
//   push ebx (end()); push esi (slot)
//   call erase(begin()+_Newsize, end())    ; this in ecx (kept)
//   jmp END
// INSERT (index >= count):
//   mov ebx,[ecx+4]       ; _Last reloaded (end())
//   push [esp+0x14]       ; &_Val (const T& reference, one dword)
//   sub eax,edi via recomputed span; push 0x54; pop edi; cdq; idiv edi  ; size()
//   sub esi,eax           ; _Newsize - size()
//   push esi; push ebx (end())
//   call _Insert_n(end(), _Newsize-size(), _Val)   ; this in ecx (kept)
// END:
//   pop edi; pop esi; pop ebx; ret 8
//
// Element stride is 0x54 (84) bytes, so the (_Last-_First) pointer difference is
// a real signed division by 0x54 (cl materialises the 0x54 divisor via
// `push 0x54; pop reg` and the begin()+index scaling via `imul reg,reg,0x54`).
// _Val is passed by const reference (single dword pushed; ret 8).  erase and
// _Insert_n are members of the same container (this in ecx), declared but not
// defined here so cl emits the direct relocation-masked calls with ecx kept as
// `this`.

struct Pair {
    unsigned long first;   // +0x00
    char          second[80];  // +0x04 .. +0x53
};                         // sizeof == 0x54 (84), align 4

struct CArrayPair {
    Pair* _First;   // +0x00
    Pair* _Last;    // +0x04

    Pair* begin() const { return _First; }
    Pair* end() const   { return _Last; }
    unsigned int size() const { return (unsigned int)(_Last - _First); }

    // Out-of-line members -> direct __fastcall calls, ecx = this.
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
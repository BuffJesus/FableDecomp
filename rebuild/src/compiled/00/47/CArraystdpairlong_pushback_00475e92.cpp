#pragma optimize("s",on)
// Byte-exact reconstruction of retail 0x00475e92
// CArray<std::pair<long,long> >::resize  (manifest labels it "push_back";
// the body is the classic resize(_Newsize, const T&) dispatch).
//
// Retail disasm:
//   mov edx,[ecx]          ; _First   (this+0x00)
//   mov eax,[esp+4]        ; _Newsize (stack arg 1)
//   push esi; push edi
//   mov esi,[ecx+4]        ; _Last    (this+0x04)
//   mov edi,esi; sub edi,edx; sar edi,3   ; size() = (_Last-_First)/8
//   cmp eax,edi; jae INSERT ; _Newsize < size() -> ERASE, else INSERT
// ERASE:
//   push esi               ; end()
//   lea eax,[edx+eax*8]    ; begin()+_Newsize
//   push eax
//   call erase(begin()+_Newsize, end())      ; this in ecx (kept)
//   jmp END
// INSERT:
//   mov esi,[ecx+4]        ; _Last reloaded (end())
//   push [esp+0x10]        ; &_Val (const T& reference, one dword)
//   mov edi,esi; sub edi,edx; sar edi,3      ; size() recomputed
//   sub eax,edi            ; _Newsize - size()
//   push eax
//   push esi               ; end()
//   call _Insert_n(end(), _Newsize-size(), _Val)   ; this in ecx (kept)
// END:
//   pop edi; pop esi; ret 8
//
// Element stride is 8 bytes (std::pair<long,long>).  _Val is passed by const
// reference (single dword pushed; ret 8).  erase / _Insert_n are members of the
// same container (this in ecx), declared but not defined here so cl emits the
// direct relocation-masked calls with ecx preserved as `this`.

struct Pair {
    long first;   // +0
    long second;  // +4
};                // sizeof == 8

struct CArrayPair {
    Pair* _First;  // +0x00
    Pair* _Last;   // +0x04

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
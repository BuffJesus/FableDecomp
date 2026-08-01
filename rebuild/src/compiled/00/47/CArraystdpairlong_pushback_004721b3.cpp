#pragma optimize("s",on)
// Byte-exact GENUINE-C++ reconstruction of retail 0x004721b3
// CArray<T>::resize (manifest labels it "push_back"; the body is the classic
// resize(_Newsize, const T&) dispatch).  Element stride is 4 bytes here, so
// T is a single 4-byte word (one `long`) -> pointer arithmetic scales by 4 and
// the size() shift the compiler emits is `sar edi,2`.
//
// This is the exact same source shape as the already-landed byte-matching
// sibling 0x00475e92 (which is stride 8, `sar edi,3`); the only change is the
// element size, which the compiler turns into the /4 stride automatically.
//
// Retail disasm:
//   mov edx,[ecx]          ; _First   (this+0x00)
//   mov eax,[esp+4]        ; _Newsize (stack arg 1, unsigned)
//   push esi; push edi
//   mov esi,[ecx+4]        ; _Last    (this+0x04)
//   mov edi,esi; sub edi,edx; sar edi,2   ; size() = (_Last-_First)/4
//   cmp eax,edi; jae INSERT               ; _Newsize < size() -> ERASE, else INSERT
// ERASE:
//   push esi               ; end()
//   lea eax,[edx+eax*4]    ; begin()+_Newsize
//   push eax
//   call erase(begin()+_Newsize, end())   ; this in ecx (kept)
//   jmp END
// INSERT:
//   mov esi,[ecx+4]        ; _Last reloaded (end())
//   push [esp+0x10]        ; &_Val (const T& reference, one dword)
//   mov edi,esi; sub edi,edx; sar edi,2   ; size() recomputed
//   sub eax,edi            ; _Newsize - size()
//   push eax
//   push esi               ; end()
//   call _Insert_n(end(), _Newsize-size(), _Val)   ; this in ecx (kept)
// END:
//   pop edi; pop esi; ret 8
//
// _Val is passed by const reference (single dword pushed; ret 8).  erase /
// _Insert_n are members of the same container (this in ecx), declared but not
// defined here so cl emits the direct relocation-masked calls with ecx
// preserved as `this`.

struct Elem {
    long value;   // +0
};                // sizeof == 4

struct CArrayElem {
    Elem* _First;  // +0x00
    Elem* _Last;   // +0x04

    Elem* begin() const { return _First; }
    Elem* end() const   { return _Last; }
    unsigned int size() const { return (unsigned int)(_Last - _First); }

    // Out-of-line members -> direct __fastcall calls, ecx = this.
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
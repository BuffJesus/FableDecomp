#pragma optimize("s",on)
// Byte-exact reconstruction of retail 0x00472b9d
// CArray<std::pair<long,...> >::resize  (manifest labels it "push_back";
// the body is the classic resize(_Newsize, const T&) dispatch, identical in
// shape to the landed sibling 0x00475e92 but over a 64-byte element).
//
// Retail disasm:
//   mov edx,[ecx]          ; _First   (this+0x00)
//   mov eax,[esp+4]        ; _Newsize (stack arg 1)
//   push esi; push edi
//   mov esi,[ecx+4]        ; _Last    (this+0x04)
//   mov edi,esi; sub edi,edx; sar edi,6   ; size() = (_Last-_First)/0x40
//   cmp eax,edi; jae INSERT ; _Newsize < size() -> ERASE, else INSERT
// ERASE:
//   shl eax,6              ; _Newsize * 0x40
//   push esi               ; end()
//   add eax,edx            ; begin() + _Newsize   (stride 0x40 -> shl+add, not lea)
//   push eax
//   call erase(begin()+_Newsize, end())      ; this in ecx (kept)
//   jmp END
// INSERT:
//   mov esi,[ecx+4]        ; _Last reloaded (end())
//   push [esp+0x10]        ; &_Val (const T& reference, one dword)
//   mov edi,esi; sub edi,edx; sar edi,6      ; size() recomputed
//   sub eax,edi            ; _Newsize - size()
//   push eax
//   push esi               ; end()
//   call _Insert_n(end(), _Newsize-size(), _Val)   ; this in ecx (kept)
// END:
//   pop edi; pop esi; ret 8
//
// Element stride is 0x40 (64 bytes).  Because 0x40 is not an lea scale (max 8),
// begin()+_Newsize compiles to `shl eax,6 ; add eax,edx` instead of the sibling's
// single lea.  _Val is passed by const reference (single dword pushed; ret 8).
// erase / _Insert_n are members of the same container (this in ecx), declared but
// not defined here so cl emits the direct relocation-masked calls with ecx kept.

struct Elem {
    // std::pair<long, ...> whose full second member sums to a 64-byte element.
    long  first;    // +0x00
    char  _pad[60]; // +0x04 .. +0x3f
};                  // sizeof == 0x40

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
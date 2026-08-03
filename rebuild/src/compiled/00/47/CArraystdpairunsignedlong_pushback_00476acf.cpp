#pragma optimize("s",on)
// Byte-exact GENUINE-C++ reconstruction of retail 0x00476acf.
//
// The manifest labels this "push_back", but the body is the classic
// CArray<T>::resize(_Newsize, const T&) dispatch -- identical in shape to the
// already-landed byte-matching sibling 0x00475e92
// (CArraystdpairlong_pushback_00475e92.cpp), only the element STRIDE differs:
// here sizeof(T) == 0x4c (76) instead of 8, so the compiler cannot fold the
// element-count division into a `sar` and emits a real signed `idiv 0x4c`.
//
// Retail disasm mapping (this in ecx: _First @ +0x00, _Last @ +0x04):
//   mov ebx,[ecx+4]        ; _Last (end())
//   mov edi,[ecx]          ; _First (begin())
//   eax = _Last - _First ; cdq ; idiv 0x4c   ; size() = (_Last-_First)/0x4c
//   mov esi,[esp+0x10]     ; _Newsize (stack arg 1, /Oy frame-pointer omitted)
//   cmp esi,eax ; jae INSERT                  ; _Newsize >= size() -> INSERT
// ERASE (_Newsize < size()):
//   imul esi,esi,0x4c ; add esi,edi           ; begin() + _Newsize
//   push _Last(ebx) ; push esi ; call erase    ; ecx (this) preserved
//   jmp END
// INSERT:
//   mov ebx,[ecx+4]        ; _Last reloaded (end())
//   push [esp+0x14]        ; &_Val (const T& -> one dword)
//   eax = _Last - _First ; cdq ; idiv 0x4c     ; size() recomputed
//   sub esi,eax           ; _Newsize - size()
//   push esi ; push _Last(ebx) ; call _Insert_n ; ecx (this) preserved
// END:
//   pop edi ; pop esi ; pop ebx ; ret 8
//
// erase / _Insert_n are out-of-line members of the same container (this in ecx),
// declared but not defined here, so cl emits the direct relocation-masked calls
// with ecx kept as `this`.  _Val is a const reference (single dword pushed;
// ret 8 cleans the two stack args _Newsize and &_Val).

struct Elem {
    char raw[0x4c];   // sizeof == 76 (0x4c) -> forces idiv, not sar
};

struct CArrayPair {
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

void CArrayPair::resize(unsigned int _Newsize, const Elem& _Val)
{
    if (_Newsize < size())
        erase(begin() + _Newsize, end());
    else
        _Insert_n(end(), _Newsize - size(), _Val);
}
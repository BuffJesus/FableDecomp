#pragma optimize("s",on)
// Byte-exact reconstruction of retail 0x00479595
// CArray<std::pair<unsigned_long,...> >::push_back  (manifest label);
// the body is the classic resize(_Newsize, const T&) dispatch, identical in
// shape to the stride-8 sibling 0x00475e92 but with a 0x50-byte (80-byte)
// element, so pointer subtraction lowers to a signed `idiv 0x50` instead of a
// `sar` and the begin()+index address math lowers to `lea [esi+esi*4]; shl 4`
// (index*5*16 == index*0x50).  Byte-for-byte the same shape as sibling
// 0x00476123 -- only the two relocation-masked `call` targets differ.
//
// Retail disasm (annotated):
//   mov ebx,[ecx+4]       ; _Last   (this+0x04)
//   mov edi,[ecx]         ; _First  (this+0x00)
//   mov eax,ebx; sub eax,edi ; byte span (_Last-_First)
//   push 0x50; cdq; pop esi; idiv esi ; size() = span/0x50  (signed idiv)
//   mov esi,[esp+0x10]    ; _Newsize (stack arg 1)
//   cmp esi,eax; jae INSERT   ; _Newsize < size() -> ERASE else INSERT
// ERASE:
//   lea eax,[esi+esi*4]; shl eax,4 ; _Newsize*0x50
//   push ebx              ; end()
//   add eax,edi           ; begin()+_Newsize
//   push eax
//   call erase(begin()+_Newsize, end())   ; this in ecx (kept)
//   jmp END
// INSERT:
//   mov ebx,[ecx+4]       ; _Last reloaded (end())
//   push [esp+0x14]       ; &_Val (const T& reference, one dword)
//   mov eax,ebx; sub eax,edi
//   push 0x50; pop edi; cdq; idiv edi ; size() recomputed
//   sub esi,eax           ; _Newsize - size()
//   push esi
//   push ebx              ; end()
//   call _Insert_n(end(), _Newsize-size(), _Val)  ; this in ecx (kept)
// END:
//   pop edi; pop esi; pop ebx; ret 8
//
// Element stride is 0x50 bytes.  _Val is passed by const reference (single dword
// pushed; ret 8).  erase / _Insert_n are members of the same container (this in
// ecx), declared but not defined here so cl emits the direct relocation-masked
// calls with ecx preserved as `this`.

// 0x50-byte POD element (std::pair<unsigned long, big-payload>).  sizeof == 0x50
// forces the signed idiv for pointer subtraction and the lea/shl for scaling.
struct Elem {
    unsigned long m[20];   // 20 * 4 == 0x50 bytes
};                         // sizeof == 0x50

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
// Faithful VC7.1 reconstruction of the _Dest_val destroy helper at 004e74f8.
// Disasm:
//   push esi; mov esi,ecx
//   lea ecx,[esi+0x24]; call 0x4b8940     ; dtor of member at +0x24
//   lea ecx,[esi+0x0c]; call 0x4e34ec     ; dtor of member at +0x0c
//   mov ecx,esi; pop esi; jmp 0x686830    ; tail-call dtor of head at +0
//
// This is _Dest_val<allocator<pair<CCharString,CSymbolInfo>>, pair<...>>:
// it destroys the pointed-to pair in place. The compiler inlined the pair's
// second member (CSymbolInfo, members at +0x0c and +0x24) and tail-calls the
// CCharString dtor of the first member at +0x0.

struct HeadStr {          // CCharString-like head at +0x00
    char pad[0x0c];
    ~HeadStr();           // -> 0x686830 (tail-called)
};
struct MidObj {           // member at +0x0c
    char pad[0x18];
    ~MidObj();            // -> 0x4e34ec
};
struct TailObj {          // member at +0x24
    char pad[4];
    ~TailObj();           // -> 0x4b8940
};

struct PairType {
    HeadStr first;        // +0x00
    MidObj  mid;          // +0x0c
    TailObj tail;         // +0x24
};

// Free fastcall helper: ecx = pointer to the pair to destroy.
// Name carries the record leaf token (CSymbolInfo___) so the harness selects
// this symbol rather than the compiler's scalar-deleting-destructor thunks.
void __fastcall _Dest_val_CSymbolInfo___(PairType *self)
{
    self->tail.~TailObj();   // +0x24  -> 0x4b8940
    self->mid.~MidObj();     // +0x0c  -> 0x4e34ec
    self->first.~HeadStr();  // +0x00  -> 0x686830 (tail-call)
}
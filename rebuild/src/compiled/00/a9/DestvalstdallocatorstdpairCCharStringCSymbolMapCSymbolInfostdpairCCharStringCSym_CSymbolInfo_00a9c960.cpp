// Retail 0x00a9c960:
//   _Dest_val<allocator<pair<CCharString,CSymbolMap::CSymbolInfo>>,
//             pair<CCharString,CSymbolMap::CSymbolInfo> >
// This is the std allocator destroy-in-place helper; the pair destructor is
// inlined into it.  The pair value has two trailing CCharString members at
// +0x30 / +0x34 and a leading "first" subobject at offset 0.
//   0x99eae0 = CCharString::~CCharString
//   0xaa8490 = FirstPart::~FirstPart   (tail call, ecx = the pair)
//
// Retail body:
//   push esi ; mov esi,ecx
//   lea ecx,[esi+0x34] ; call 0x99eae0     // ~s1
//   lea ecx,[esi+0x30] ; call 0x99eae0     // ~s0
//   mov ecx,esi ; pop esi ; jmp 0xaa8490   // ~first (tail)

// 4-byte CCharString (buffer pointer). Non-inline dtor -> real call.
struct CCharString {
    char* buf;
    ~CCharString();
};

// Leading subobject occupying offsets 0..0x30; its dtor is tail-called.
struct FirstPart {
    char pad[0x30];
    ~FirstPart();
};

// The pair value: FirstPart @0, then two CCharString members @0x30 / @0x34.
struct PairVal {
    FirstPart first;
    CCharString s0;   // +0x30
    CCharString s1;   // +0x34
};

// _Dest_val: destroy the pair in place (self in ecx). Member/base destruction
// in reverse declaration order: s1 (@0x34), s0 (@0x30), then first (@0)
// tail-destroyed.  __fastcall so the object pointer arrives in ecx like retail.
void __fastcall _Dest_val_CSymbolInfo___(PairVal* self)
{
    self->~PairVal();
}
// Retail 0x004c3360
// _Dest_val<allocator<pair<CCharString,CSymbolMap::CSymbolInfo> >, pair<...> >
//
// Body:
//   push esi; mov esi,ecx
//   lea ecx,[esi+0x30]; call 0x99a2e0   ; CCharString::~CCharString on sub at +0x30
//   lea ecx,[esi+0x14]; call 0x99a2e0   ; CCharString::~CCharString on sub at +0x14
//   mov ecx,esi; pop esi; jmp 0x4c2e90  ; tail-call pair-level dtor on self
//
// The two identical calls destroy two CCharString subobjects (at +0x14 and
// +0x30); the tail-jmp runs the pair-level destructor on self. All three are
// external non-virtual __fastcall helpers; the harness masks the relocation
// dwords so any consistent targets reproduce the bytes.

struct CCharString;
struct SymbolPair;

// ecx = this. Modeled as single-arg free __fastcall (self in ecx, edx unused).
extern "C" void __fastcall CCharString_Dtor(CCharString *self); // 0x99a2e0
extern "C" void __fastcall SymbolPair_Dtor(SymbolPair *self);   // 0x4c2e90

struct CCharString {};

struct SymbolPair
{
    char        pad0[0x14];   // 0x00 .. 0x13  (pair.first CCharString lives here)
    CCharString strA;         // 0x14  (subobject destroyed second)
    char        pad1[0x30 - 0x14 - 1];
    CCharString strB;         // 0x30  (subobject destroyed first)
};

// self (allocator/pair base) arrives in ecx; the second pair* param (edx) is
// unused by the body, matching retail.
void __fastcall _Dest_val_SymbolPair(SymbolPair *self, SymbolPair *unused)
{
    CCharString_Dtor(&self->strB);   // lea ecx,[esi+0x30]; call
    CCharString_Dtor(&self->strA);   // lea ecx,[esi+0x14]; call
    SymbolPair_Dtor(self);           // mov ecx,esi; jmp (tail call)
}
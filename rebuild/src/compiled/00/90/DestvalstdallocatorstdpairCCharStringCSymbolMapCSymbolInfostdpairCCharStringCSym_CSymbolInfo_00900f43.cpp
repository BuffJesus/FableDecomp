// Byte-exact reconstruction of retail 0x00900f43
//   std::_Dest_val<allocator<pair<CCharString,CSymbolMap::CSymbolInfo>>, pair<...>>
// Retail shape:
//   push esi; mov esi,ecx
//   lea ecx,[esi+0x1c]; call CCharString::~CCharString
//   lea ecx,[esi+0x14]; call CCharString::~CCharString
//   mov ecx,esi; pop esi; jmp 0x8fd288   (tail-call: destroy the pair's first member)
//
// Model: a member function of the pair object.  ecx = pair*.  The two members at
// +0x14 and +0x1c are CCharString sub-objects that get destroyed, then a tail
// call to the "first"-destroy helper on the same object.

struct CCharString {
    char* m_ptr;   // +0x00
    int   m_len;   // +0x04
    // real retail dtor lives at 0x00a01c10; sub-objects sit 8 bytes apart
    void Destroy();   // out-of-line, non-virtual
};

// tail-call target 0x8fd288: destroys the pair's first CCharString (+0x08 region)
struct Pair;
void __fastcall PairDestroyFirst(Pair* self);   // 0x8fd288

// pair<CCharString, CSymbolMap::CSymbolInfo>
//   +0x08 .. : first  (CCharString)  -> handled by PairDestroyFirst
//   +0x14    : CSymbolInfo member CCharString #1
//   +0x1c    : CSymbolInfo member CCharString #2
struct Pair {
    char       pad0[0x14];   // 0x00 .. 0x13
    CCharString s_a;         // +0x14
    CCharString s_b;         // +0x1c
};

// _Dest_val destroys the pair sub-objects.  Modeled as a member so 'this' = ecx.
void __fastcall Dest_val(Pair* self)
{
    self->s_b.Destroy();     // lea ecx,[esi+0x1c]; call ~CCharString
    self->s_a.Destroy();     // lea ecx,[esi+0x14]; call ~CCharString
    PairDestroyFirst(self);  // tail jmp 0x8fd288
}
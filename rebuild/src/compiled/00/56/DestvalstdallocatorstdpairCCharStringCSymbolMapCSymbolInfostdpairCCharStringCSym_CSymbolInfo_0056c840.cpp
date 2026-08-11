// Faithful VC7.1 reconstruction of _Dest_val<...> at 0x0056c840
//
// Retail disasm:
//   push esi; mov esi,ecx
//   lea ecx,[esi+0x18]; call 0x99b510   (~CCharString on member@0x18)
//   lea ecx,[esi+0x14]; call 0x99b510   (~CCharString on member@0x14)
//   lea ecx,[esi+0x10]; pop esi
//   jmp 0x99eae0                         (tail ~T on member@0x10)
//
// This is the STL _Dest_val helper that destroys a
// pair<CCharString, CSymbolMap::CSymbolInfo>.  The pair object begins at
// [ecx].  Only the sub-object destructors at offsets 0x18, 0x14, 0x10 are
// invoked (members below 0x10 are trivially-destructible scalars, so no
// dtor code is emitted for them).  The last dtor is tail-called.

// ---- CCharString: destroyed via 0x99b510 (non-trivial, has vtable/heap) ----
struct CCharString {
    char* m_buf;              // +0x00
    ~CCharString();           // -> 0x99b510
};

// ---- The type whose dtor is 0x99eae0 (member@0x10 of the pair) ----
struct CInfoHead {
    int   m_a;                // +0x00
    ~CInfoHead();             // -> 0x99eae0
};

// pair<CCharString, CSymbolInfo> memory layout.
// first (CCharString) + second (CSymbolInfo).  We only need the three
// sub-objects that carry non-trivial destructors, at 0x10 / 0x14 / 0x18,
// preceded by 0x10 bytes of trivially-destructible scalar fields.
struct SymbolPair {
    int       m_pad[4];       // +0x00 .. +0x0F  (trivial scalars)
    CInfoHead m_head;         // +0x10  (dtor 0x99eae0, tail-called last)
    CCharString m_s1;         // +0x14  (dtor 0x99b510)
    CCharString m_s2;         // +0x18  (dtor 0x99b510)
};

// _Dest_val: __fastcall(allocator*, pair*).  The allocator arrives in ecx
// but the emitted body destroys the object *at ecx* (the pair pointer is the
// operand actually used).  Model it as a free __fastcall whose first arg is
// the object being destroyed; the second (edx) allocator arg is unused.
void __fastcall Dest_val_CSymbolInfo___(SymbolPair* self, int /*edx*/)
{
    self->m_s2.~CCharString();   // +0x18
    self->m_s1.~CCharString();   // +0x14
    self->m_head.~CInfoHead();   // +0x10  (tail)
}
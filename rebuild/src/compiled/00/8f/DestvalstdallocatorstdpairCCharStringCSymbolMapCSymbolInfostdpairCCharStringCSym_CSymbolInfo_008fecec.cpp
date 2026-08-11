// _Dest_val< allocator< pair<CCharString,CSymbolMap::CSymbolInfo> >, pair<...> >
// Retail 0x008fecec. The empty stateless allocator collapses _Dest_val to
// _Ptr->~pair(); the pair pointer arrives in ecx. The pair destructor:
//   - destroys the two CCharStrings inside 'second' (CSymbolInfo) at +0x14/+0x1c
//   - tail-jumps to the base/first-member destructor with this (0x8fd288)
// The wrapper class is named to carry the record's leaf token so the harness
// picks the right .text block from the object file.

// CCharString: a small string wrapper; its dtor is the leaf at 0xa01c10.
struct CCharString {
    void* p;          // +0x00 : buffer / refcounted pointer
    ~CCharString();   // out-of-line; retail 0xa01c10
};

// Models the pair<CCharString, CSymbolMap::CSymbolInfo> whose _Dest_val this is.
// 'first' CCharString at +0x00 (base dtor at 0x8fd288 via tail jmp with this);
// the CSymbolInfo 'second' contributes the two CCharStrings at +0x14 and +0x1c.
struct CSymbolInfo___ {
    CCharString first;    // +0x00
    int         pad0;     // +0x04
    int         pad1;     // +0x08
    int         c0;       // +0x0c
    int         c1;       // +0x10
    CCharString s1;       // +0x14
    int         gap;      // +0x18
    CCharString s2;       // +0x1c
    void Dest_val();      // the destroy helper (this in ecx)
    void PairFirstDtor(); // the first-member/base dtor at 0x8fd288 (this in ecx)
};

void CSymbolInfo___::Dest_val()
{
    this->s2.~CCharString();   // lea ecx,[esi+0x1c]; call 0xa01c10
    this->s1.~CCharString();   // lea ecx,[esi+0x14]; call 0xa01c10
    this->PairFirstDtor();     // mov ecx,esi; jmp 0x8fd288
}
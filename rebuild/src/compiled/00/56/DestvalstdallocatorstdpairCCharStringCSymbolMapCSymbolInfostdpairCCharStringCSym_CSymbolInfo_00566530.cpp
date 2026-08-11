// _Dest_val for std::allocator<std::pair<CCharString, CSymbolMap::CSymbolInfo> >
// Retail 0x00566530. Destroys a pair<CWideString, CSymbolInfo> in place.
//   [esi+0xc] dtor 0x99eae0 ; [esi+8] dtor 0x99b510 (CWideString) ; [esi+0] dtor 0x99b510 (tail)

struct CWideString {            // dtor 0x99b510 ; 4 bytes
    unsigned short* m_pData;
    ~CWideString();
};

struct CSymOther {              // dtor 0x99eae0
    int a;
    ~CSymOther();
};

// Flattened pair layout: first @+0, pad @+4, second.m_name @+8, second.m_x @+0xc.
struct SymPair {
    CWideString first;          // +0, dtor 0x99b510
    int         _pad;           // +4 (pair 'second' aligns to +8)
    CWideString sec_name;       // +8, dtor 0x99b510
    CSymOther   sec_x;          // +0xc, dtor 0x99eae0
};

// ecx = pair pointer. Reverse-order member destruction, as std::pair would emit.
void __fastcall _Dest_val_CSymbolInfo___SymPair(SymPair* p)
{
    p->sec_x.~CSymOther();      // +0xc  -> 0x99eae0
    p->sec_name.~CWideString(); // +8    -> 0x99b510
    p->first.~CWideString();    // +0    -> 0x99b510 (tail)
}
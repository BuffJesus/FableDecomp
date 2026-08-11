// _Dest_val<allocator<pair<CCharString,CSymbolMap::CSymbolInfo>>, pair<...>>
// Retail 0x004af220 (28 bytes):
//   push esi; mov esi,ecx
//   lea ecx,[esi+0x14]; call 0x99b510   (CCharString::~CCharString)
//   lea ecx,[esi+0x10]; call 0x99b510   (CCharString::~CCharString)
//   lea ecx,[esi+0x0c]; pop esi; jmp 0x99eae0  (tail: +0xc member dtor)
//
// This is the pair<CCharString,CSymbolInfo> destructor with its member
// destructors inlined (the allocator _Dest_val helper devolves to it).
// Members destroyed in REVERSE declaration order: +0x14, +0x10, +0x0c.

// CCharString: out-of-line dtor -> the 0x99b510 call. 3 dwords = 0xc bytes,
// which gives the +0x10 -> +0x14 spacing.
struct CCharString {
    char* p;              // +0x00  (single pointer -> 4-byte member)
    ~CCharString();       // out-of-line
};

// The +0x0c member with its own out-of-line dtor -> the tail jmp 0x99eae0.
struct CSymbolInfo {
    int slot;             // +0x00 of this member
    ~CSymbolInfo();       // out-of-line
};

// pair node laid out so second=+0x0c, then two CCharStrings at +0x10,+0x14.
struct SymPair {
    char        head[0x0c];   // +0x00..+0x0b
    CSymbolInfo second;       // +0x0c  (declared first -> destroyed LAST / tail)
    CCharString a;            // +0x10  (destroyed 2nd)
    CCharString b;            // +0x14  (destroyed 1st)
    // Name carries the harness leaf token (the record's final "::"-segment
    // is CSymbolInfo___); keeps this fn uniquely selected over the compiler's
    // scalar-deleting dtors (??_G...) which contain only "CSymbolInfo".
    void _Dest_val_CSymbolInfo___();
};

// The body: reverse-order member destruction, inlined -> exactly retail.
void SymPair::_Dest_val_CSymbolInfo___() {
    this->b.~CCharString();       // +0x14  call 0x99b510
    this->a.~CCharString();       // +0x10  call 0x99b510
    this->second.~CSymbolInfo();  // +0x0c  tail jmp 0x99eae0
}
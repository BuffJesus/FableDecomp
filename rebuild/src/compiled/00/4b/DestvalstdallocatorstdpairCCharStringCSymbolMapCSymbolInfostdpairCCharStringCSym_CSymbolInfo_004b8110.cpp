// Retail 0x004b8110 : std::_Dest_val< allocator<pair<CCharString,CSymbolInfo>>,
//                                      pair<CCharString,CSymbolInfo> >
// Disasm (this=ecx=pair):
//   push esi; mov esi,ecx
//   lea ecx,[esi+0x14]; call 0x99b510      ; ~member(+0x14)
//   lea ecx,[esi+0x10]; call 0x99b510      ; ~member(+0x10)
//   lea ecx,[esi+0x0c]; pop esi; jmp 0x99eae0  ; tail ~member(+0x0c)
//
// The three destroyed members sit at CONSECUTIVE 4-byte offsets 0x0c/0x10/0x14,
// so each is a 4-byte (single-pointer) object with a destructor. Members are
// destroyed in reverse declaration order -> declare ascending by offset.

struct StrRefA {   // 4 bytes; dtor -> 0x99b510
    void* p;
    ~StrRefA();
};

struct StrRefB {   // 4 bytes; dtor -> 0x99eae0
    void* p;
    ~StrRefB();
};

struct SymPair_Dest_val {
    char     pad0[0x0c]; // +0x00..+0x0b : no destructor emitted
    StrRefB  m0c;        // +0x0c : destroyed last  (tail 0x99eae0)
    StrRefA  m10;        // +0x10 : destroyed second (0x99b510)
    StrRefA  m14;        // +0x14 : destroyed first  (0x99b510)
    ~SymPair_Dest_val();
};

SymPair_Dest_val::~SymPair_Dest_val() {}
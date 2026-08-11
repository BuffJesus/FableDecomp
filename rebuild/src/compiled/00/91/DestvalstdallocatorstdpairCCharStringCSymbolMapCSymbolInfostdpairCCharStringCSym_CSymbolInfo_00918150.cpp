// _Dest_val for std::allocator<std::pair<CCharString, CSymbolMap::CSymbolInfo>>
// Retail 0x00918150. The pair pointer arrives in ecx (this-in-ecx); allocator arg unused.
//   push esi; mov esi,ecx
//   lea ecx,[esi+0x14]; call 0xa01c10   (dtor of sub-object at +0x14)
//   lea ecx,[esi+0x0c]; call 0x99eae0   (dtor of sub-object at +0x0c)
//   mov ecx,esi; pop esi; jmp 0x99a2e0  (tail dtor of first CCharString at +0x0)

struct CCharString {
    char* buf;      // +0
    int   a;        // +4
    int   b;        // +8
    ~CCharString(); // 0x0099a2e0
};

struct SubB {
    char* p;   // +0xC
    int   q;   // +0x10
    ~SubB();   // 0x0099eae0
};

struct SubA {
    char* p;   // +0x14
    int   q;   // +0x18
    int   r;   // +0x1C
    ~SubA();   // 0x00a01c10
};

struct Pair {
    CCharString first; // +0x0
    SubB        b;      // +0xC
    SubA        a;      // +0x14
};

// this-in-ecx = pair pointer; second (unused) param = allocator/edx.
void __fastcall _Dest_val_CSymbolInfo___(Pair* _Ptr, int /*edx*/)
{
    _Ptr->a.~SubA();   // +0x14  -> 0x00a01c10
    _Ptr->b.~SubB();   // +0x0c  -> 0x0099eae0
    _Ptr->first.~CCharString(); // +0x0 -> 0x0099a2e0 (tail)
}
// Faithful VC7.1 reconstruction of std::_Dest_val for
//   std::pair<CCharString, CSymbolMap::CSymbolInfo>
// Retail 0x007699c0. Pair + CSymbolInfo destructors inline; leaf CCharString /
// CBaseIntelligentPointer destructors are out-of-line -> call/jmp.

class CCharString {
public:
    ~CCharString();          // 0x99eae0
    char* m_p;               // sizeof == 4
};

class CBaseIntelligentPointer {
public:
    ~CBaseIntelligentPointer();  // 0xa01c10 (direct member destruction)
    void* m_p;
};

struct CSymbolInfo {
    CCharString name;             // +0
    int a;                        // +4
    int b;                        // +8
    CBaseIntelligentPointer ptr;  // +0xC
};

struct SymPair {
    CCharString first;    // +0
    CSymbolInfo second;   // +4  -> name +4, ptr +0x10
};

// std::_Dest_val<Alloc, pair>(Alloc&, pair* _Ptr) { _Ptr->~pair(); }
// ecx carries _Ptr in retail.
void __fastcall Dest_val_CSymbolInfo___(SymPair* _Ptr)
{
    _Ptr->second.ptr.~CBaseIntelligentPointer();
    _Ptr->second.name.~CCharString();
    _Ptr->first.~CCharString();
}
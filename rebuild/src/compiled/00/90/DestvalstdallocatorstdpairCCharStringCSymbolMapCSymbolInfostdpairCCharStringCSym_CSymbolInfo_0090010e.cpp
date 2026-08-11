// Faithful VC7.1 reconstruction of
// _Dest_val<allocator<pair<CCharString,CSymbolMap::CSymbolInfo> >, pair<...> >
// retail 0x0090010e
//
// Body destroys a pair<CCharString, CSymbolInfo> in reverse member order:
//   two CCharString members inside CSymbolInfo (at +0x1c and +0x14) -> 0xa01c10
//   then tail-calls the leading first-member dtor (at +0) -> 0x8fd288

class CCharString
{
public:
    void Destroy();   // out-of-line CCharString dtor (0xa01c10)
    char *m_pData;
};

class CFirstMember
{
public:
    void Destroy();   // out-of-line first-member dtor (0x8fd288)
    char *m_pData;
};

struct SymPair
{
    CFirstMember first;          // +0x00
    int   pad0;                  // +0x04
    int   pad1;                  // +0x08
    int   pad2;                  // +0x0c
    int   pad3;                  // +0x10
    CCharString second_name0;    // +0x14
    int   pad4;                  // +0x18
    CCharString second_name1;    // +0x1c
};

void __fastcall Dest_val_SymPair(SymPair *self)
{
    self->second_name1.Destroy();  // +0x1c  call 0xa01c10
    self->second_name0.Destroy();  // +0x14  call 0xa01c10
    self->first.Destroy();         // +0x00  tail jmp 0x8fd288
}
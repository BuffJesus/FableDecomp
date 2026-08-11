// Retail 0x00cd4600:
//   std::allocator<std::pair<CCharString,CSymbolMap::CSymbolInfo> >::_Dest_val
// Destroys a pair<CCharString,CSymbolInfo> in reverse member order. The pair
// holds three CCharString-destructible members at +0,+4,+8; each is torn down
// by CCharString::~CCharString (retail 0x0099eae0, a this-in-ecx call). The
// final dtor call is a tail-call (mov ecx,esi; pop esi; jmp).

struct CCharString {
    char* p;
};

// CCharString::~CCharString (retail 0x0099eae0). Modeled as a this-in-ecx call
// via __fastcall (ecx=self, edx unused) so no scalar-deleting-destructor helper
// is generated into this translation unit.
void __fastcall CCharStringDtor(CCharString* self);

// pair<CCharString, CSymbolMap::CSymbolInfo> layout, from the teardown order:
// destructible sub-objects at +0, +4, +8.
struct PairCharStringSymbolInfo {
    CCharString m0;   // +0
    CCharString m1;   // +4
    CCharString m2;   // +8
};

// std::allocator<...>::_Dest_val destroys the object pointed to by ecx. The
// allocator is a stateless empty type, so _Dest_val simply runs the pair's
// member destructors, last member first.
struct AllocDestVal_pair_CCharString_CSymbolInfo {
    void _Dest_val();
};

void AllocDestVal_pair_CCharString_CSymbolInfo::_Dest_val()
{
    PairCharStringSymbolInfo* self = reinterpret_cast<PairCharStringSymbolInfo*>(this);
    CCharStringDtor(&self->m2);
    CCharStringDtor(&self->m1);
    CCharStringDtor(&self->m0);
}
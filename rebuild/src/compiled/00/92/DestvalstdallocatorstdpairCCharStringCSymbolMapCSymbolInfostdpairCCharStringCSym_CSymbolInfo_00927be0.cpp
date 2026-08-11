// _Dest_val<allocator<pair<CCharString,CSymbolMap::CSymbolInfo> >, pair<...> >
// Retail 0x00927be0. Destroys the two internal sub-objects of the pair
// (at +0x14 and +0xc) then tail-calls the pair's own destructor.

// The pair<CCharString, CSymbolInfo> object. CSymbolInfo carries two
// destructible members (e.g. CCharStrings) at +0xc and +0x14 relative to
// the pair base; the CCharString 'first' begins the pair. The three engine
// destructors live at fixed addresses.
struct SymPair;

// engine dtor called on the member at +0x14  (retail 0x00a01c10)
void __fastcall memb14_dtor(void* self);
// engine dtor called on the member at +0xc   (retail 0x0099eae0)
void __fastcall memb0c_dtor(void* self);
// engine dtor called on the whole pair        (retail 0x0099a2e0)
void __fastcall pair_dtor(SymPair* self);

void __fastcall _Dest_val(SymPair* p, void* al)
{
    char* base = (char*)p;
    memb14_dtor(base + 0x14);
    memb0c_dtor(base + 0xc);
    pair_dtor(p);
}
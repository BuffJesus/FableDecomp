// Byte-exact reconstruction of
// _Dest_val<std::allocator<std::pair<CCharString,CSymbolMap::CSymbolInfo>>, std::pair<CCharString,CSymbolMap::CSymbolInfo>>
// at retail 0x00c1d4e0.
//
// Retail body: push esi; mov esi,ecx; then destroys three elements at esi+0, esi+0xc, esi+0x18
// (stride 0x0c) each via call 0xc258d0 (a destructor returning a float in st0, discarded by fstp).
//
// The base pointer arrives in ecx (arg1 of __fastcall). We model the element type at size 0x0c with
// a destructor that forwards to an extern __fastcall helper returning float so cl emits call;fstp st(0).

struct CSymbolInfoPair {
    char data[0x0c];
    ~CSymbolInfoPair();
};

// Element destructor at 0xc258d0: this in ecx, returns float in st0.
extern "C" float __fastcall pair_dtor(CSymbolInfoPair* self);

inline CSymbolInfoPair::~CSymbolInfoPair() {
    pair_dtor(this);
}

// _Dest_val destroys the 3 stride-0x0c elements. Base pointer in ecx (arg1).
void __fastcall Dest_val_std_allocator_pair_CCharString_CSymbolMap_CSymbolInfo___(CSymbolInfoPair* p)
{
    p[0].~CSymbolInfoPair();
    p[1].~CSymbolInfoPair();
    p[2].~CSymbolInfoPair();
}
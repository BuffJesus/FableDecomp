// Byte-exact reconstruction of _Dest_val for
// std::pair<CCharString, CSymbolMap::CSymbolInfo>.
//
// Retail 0x00900533:
//   push esi; mov esi,ecx
//   lea ecx,[esi+0x1c]; call 0xa01c10   (CCharString dtor on member @+0x1c)
//   lea ecx,[esi+0x14]; call 0xa01c10   (CCharString dtor on member @+0x14)
//   mov ecx,esi; pop esi; jmp 0x8fd288  (tail-call dtor(this))
//
// This is std::_Destroy on one pair<CCharString,CSymbolInfo> element:
// the pair holds a CCharString (first) and CSymbolInfo (second). VC7.1
// inlines the pair destructor: it destroys the two CCharString members
// (second's inner string @+0x1c, first @+0x14) then tail-calls the
// destructor for the remainder of the object (second's other members).

// CCharString: destructor lives at 0xa01c10. Give it a self ptr so the
// dtor is emitted as an out-of-line thiscall.
struct CCharString {
	char* m_ptr;
	int   m_len;
	~CCharString();
};

// The "second" tail object destroyed at 0x8fd288 with the same this ptr.
struct CSymbolInfoTail {
	int a;
	int b;
	int c;
	int d;
	int e;
	~CSymbolInfoTail();
};

// The pair layout so members land at the observed offsets:
//   +0x00 tail object (destroyed last, shares this)
//   +0x14 CCharString first
//   +0x1c CCharString second-inner
struct SymPair {
	CSymbolInfoTail tail;   // +0x00 .. +0x13
	CCharString     s14;    // +0x14
	CCharString     s1c;    // +0x1c
};

// _Dest_val<allocator<pair>, pair>: __fastcall, ecx = element ptr,
// second stack arg = allocator*. We destroy the element in place.
void __fastcall _Dest_val_CSymbolInfo___(SymPair* self, void* /*al*/) {
	self->s1c.~CCharString();
	self->s14.~CCharString();
	self->tail.~CSymbolInfoTail();
}
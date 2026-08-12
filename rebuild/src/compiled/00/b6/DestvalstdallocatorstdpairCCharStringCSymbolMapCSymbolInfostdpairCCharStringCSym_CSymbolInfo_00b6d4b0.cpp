// _Dest_val for std::pair<CCharString, CSymbolMap::CSymbolInfo>
// Retail 0x00b6d4b0: destroys sub-objects at +0xc, +0x20 (same dtor 0xac1ee0)
// then tail-calls dtor 0xa63200 for +0x34.

// A = CCharString-like member (non-virtual dtor at 0xac1ee0)
struct A {
    char* buf;   // +0
    int   a;     // +4
    int   b;     // +8
    ~A();        // non-virtual
};

// B = CSymbolInfo-ish member (non-virtual dtor at 0xa63200)
struct B {
    int x;
    ~B();
};

// The pair-like object whose members must be destroyed.
// A at +0xc, A at +0x20, B at +0x34.
struct DestPair {
    char pad0[0x0c];   // +0x00
    A    first;        // +0x0c  (12 bytes)
    char pad1[0x08];   // +0x18
    A    second;       // +0x20  (12 bytes)
    char pad2[0x08];   // +0x2c
    B    third;        // +0x34
};

// _Dest_val: ecx = object (self); destroy first, second, then third (tail).
// Name embeds the record leaf token "CSymbolInfo" for harness symbol selection.
void __fastcall Dest_val_CSymbolInfo___(DestPair* self)
{
    self->first.~A();
    self->second.~A();
    self->third.~B();
}
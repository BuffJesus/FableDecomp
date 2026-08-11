// Member subobject types whose destructors are the two external dtors.
// B = CCharString (dtor at 0x99b510), A = CSymbolInfo's other member (dtor 0x99eae0)
struct B {
    char* p;
    ~B();            // out-of-line -> external call target 0x99b510
};
struct A {
    int x;
    ~A();            // out-of-line -> external call target 0x99eae0
};

// Layout of pair<CCharString, CSymbolMap::CSymbolInfo> as observed:
//   +0x0 : B (first CCharString)          -> destroyed last (tail jmp)
//   +0x4 : padding / other
//   +0x8 : B (CSymbolInfo CCharString)    -> destroyed 2nd
//   +0xc : A (CSymbolInfo other member)   -> destroyed 1st
struct Pair {
    B m0;    // +0
    int pad; // +4
    B m8;    // +8
    A mc;    // +0xc
};

// _Dest_val: retail inlines the three subobject destructor calls in reverse
// declaration order (mc, m8, m0) with the last one tail-jumped.
void __fastcall _Dest_val_CSymbolInfo___(Pair* self)
{
    self->mc.~A();
    self->m8.~B();
    self->m0.~B();
}
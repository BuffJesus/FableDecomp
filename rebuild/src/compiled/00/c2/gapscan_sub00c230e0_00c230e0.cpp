// Free __stdcall comparison: a->[0x0] == b->[0xc].
#pragma pack(push,1)
struct A {
    int field;
};
struct B {
    char pad_0[0xc];
    int field;
};
#pragma pack(pop)
extern "C" bool __stdcall Compare(A* a, B* b) { return a->field == b->field; }
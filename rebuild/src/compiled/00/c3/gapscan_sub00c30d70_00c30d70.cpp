// Free __stdcall comparison: a->[0x0] == b->[0x8].
#pragma pack(push,1)
struct A {
    int field;
};
struct B {
    char pad_0[0x8];
    int field;
};
#pragma pack(pop)
extern "C" bool __stdcall Compare(A* a, B* b) { return a->field == b->field; }
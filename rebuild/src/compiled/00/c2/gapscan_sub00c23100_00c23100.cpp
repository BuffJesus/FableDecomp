// Free __stdcall comparison: a->[0x0] < b->[0xc].
#pragma pack(push,1)
struct A {
    unsigned int field;
};
struct B {
    char pad_0[0xc];
    unsigned int field;
};
#pragma pack(pop)
extern "C" int __stdcall Compare(A* a, B* b) { return a->field < b->field; }
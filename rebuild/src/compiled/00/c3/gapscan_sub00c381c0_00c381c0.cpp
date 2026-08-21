// Free __stdcall comparison of the member at +0x4 of two objects: a == b.
#pragma pack(push,1)
struct T {
    char pad_0[0x4];
    int field;
};
#pragma pack(pop)
extern "C" bool __stdcall Compare(T* a, T* b) { return a->field == b->field; }
// Free __stdcall unsigned comparison of the member at +0x8: a < b.
#pragma pack(push,1)
struct T {
    char pad_0[0x8];
    unsigned int field;
};
#pragma pack(pop)
extern "C" int __stdcall Compare(T* a, T* b) { return a->field < b->field; }
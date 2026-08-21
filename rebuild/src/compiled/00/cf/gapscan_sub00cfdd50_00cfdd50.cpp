// `++this->sub->counter;` -- sub-object pointer at this+0x14, counter at
// sub+0x60. __fastcall this=ecx.
#pragma pack(push,1)
struct Sub {
    char pad_0[0x60];
    int counter;
};
struct T {
    char pad_0[0x14];
    Sub* sub;
    void Bump();
};
#pragma pack(pop)
void T::Bump() { ++this->sub->counter; }
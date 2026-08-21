// Clear the member at this+0xc. __fastcall this=ecx.
#pragma pack(push,1)
struct T {
    char pad_0[0xc];
    int field;
    void Clear();
};
#pragma pack(pop)
void T::Clear() { this->field = 0; }
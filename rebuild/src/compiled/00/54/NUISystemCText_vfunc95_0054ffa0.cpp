// Clear the member at this+0x189. __fastcall this=ecx.
#pragma pack(push,1)
struct T {
    char pad_0[0x189];
    char field;
    void Clear();
};
#pragma pack(pop)
void T::Clear() { this->field = 0; }
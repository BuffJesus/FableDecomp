// Clear the two byte flags at this+0x1d / this+0x1c. __fastcall this=ecx.
#pragma pack(push,1)
struct T {
    char pad_0[0x1c];
    bool flag_b;
    bool flag_a;
    void Clear();
};
#pragma pack(pop)
void T::Clear() { this->flag_a = false; this->flag_b = false; }
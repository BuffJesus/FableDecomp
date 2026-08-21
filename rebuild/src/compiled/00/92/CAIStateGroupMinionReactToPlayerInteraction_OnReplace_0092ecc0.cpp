// Clear the two byte flags at this+0x2c / this+0x2d. __fastcall this=ecx.
#pragma pack(push,1)
struct T {
    char pad_0[0x2c];
    bool flag_a;
    bool flag_b;
    void Clear();
};
#pragma pack(pop)
void T::Clear() { this->flag_a = false; this->flag_b = false; }
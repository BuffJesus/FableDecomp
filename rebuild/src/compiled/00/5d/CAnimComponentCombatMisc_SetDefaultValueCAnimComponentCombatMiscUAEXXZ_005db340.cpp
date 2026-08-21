// Clear the two byte flags at this+0x8 / this+0x9. __fastcall this=ecx.
#pragma pack(push,1)
struct T {
    char pad_0[0x8];
    bool flag_a;
    bool flag_b;
    void Clear();
};
#pragma pack(pop)
void T::Clear() { this->flag_a = false; this->flag_b = false; }
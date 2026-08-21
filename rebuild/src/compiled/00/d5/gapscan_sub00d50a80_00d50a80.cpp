// Clear the two byte flags at this+0x1e / this+0x1c. __fastcall this=ecx.
#pragma pack(push,1)
struct T {
    char pad_0[0x1c];
    bool flag_b;
    char pad_1d[0x1];
    bool flag_a;
    void Clear();
};
#pragma pack(pop)
void T::Clear() { this->flag_a = false; this->flag_b = false; }
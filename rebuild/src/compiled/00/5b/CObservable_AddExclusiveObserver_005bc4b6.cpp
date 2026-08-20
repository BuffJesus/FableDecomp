// Member setter at this+0x70: one stack arg, `ret 4`. __fastcall this=ecx.
#pragma pack(push,1)
struct T {
    char pad[0x70];
    int field;
    void Set(int value);
};
#pragma pack(pop)
void T::Set(int value) { this->field = value; }
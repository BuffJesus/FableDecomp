// Member setter at this+0xdc: one stack arg, `ret 4`. __fastcall this=ecx.
#pragma pack(push,1)
struct T {
    char pad[0xdc];
    int field;
    void Set(int value);
};
#pragma pack(pop)
void T::Set(int value) { this->field = value; }
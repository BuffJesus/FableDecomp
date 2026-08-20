// Member setter at this+0x64: one stack arg, `ret 4`. __fastcall this=ecx.
struct T {
    char pad[0x64];
    int field;
    void Set(int value);
};
void T::Set(int value) { this->field = value; }
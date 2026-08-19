// Member getter at this+0x32c. __fastcall this=ecx.
struct T {
    char pad[0x32c];
    int field;
    int Get();
};
int T::Get() { return this->field; }
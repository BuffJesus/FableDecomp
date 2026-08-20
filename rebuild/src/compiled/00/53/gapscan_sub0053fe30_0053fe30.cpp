// Byte-member setter at this+0x179 from a stack arg. __fastcall this=ecx (ret 4).
#pragma pack(push,1)
struct T {
    char pad[0x179];
    char field;
    void Set(char value);
};
#pragma pack(pop)
void T::Set(char value) { this->field = value; }
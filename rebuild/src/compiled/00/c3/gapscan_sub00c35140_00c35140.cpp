// Member getter at this+0x4. __fastcall this=ecx. pack(1) keeps the field at
// the retail byte offset (natural alignment would shift an odd one).
#pragma pack(push,1)
struct T {
    char pad[0x4];
    int field;
    int Get(int a0, int a1);
};
#pragma pack(pop)
int T::Get(int a0, int a1) { return this->field; }
// Member getter at this+0xc. __fastcall this=ecx. pack(1) keeps the field at
// the retail byte offset (natural alignment would shift an odd one).
#pragma pack(push,1)
struct T {
    char pad[0xc];
    int field;
    int Get();
};
#pragma pack(pop)
int T::Get() { return this->field; }
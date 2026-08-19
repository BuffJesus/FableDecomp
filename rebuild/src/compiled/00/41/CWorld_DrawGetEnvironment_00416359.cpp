// Member getter at this+0x20. __fastcall this=ecx. pack(1) keeps the field at
// the retail byte offset (natural alignment would shift an odd one).
#pragma pack(push,1)
struct T {
    char pad[0x20];
    int field;
    int Get();
};
#pragma pack(pop)
int T::Get() { return this->field; }
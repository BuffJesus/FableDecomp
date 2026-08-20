// Member getter at this+0x1c. __fastcall this=ecx. pack(1) keeps the field at
// the retail byte offset (natural alignment would shift an odd one).
#pragma pack(push,1)
struct T {
    char pad[0x1c];
    char field;
    char Get();
};
#pragma pack(pop)
char T::Get() { return this->field; }
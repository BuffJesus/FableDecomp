// Member getter at this+0xa8. __fastcall this=ecx. pack(1) keeps the field at
// the retail byte offset (natural alignment would shift an odd one).
#pragma pack(push,1)
struct T {
    char pad[0xa8];
    char field;
    char Get();
};
#pragma pack(pop)
char T::Get() { return this->field; }
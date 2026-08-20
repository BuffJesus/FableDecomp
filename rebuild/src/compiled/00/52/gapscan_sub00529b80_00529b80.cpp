// Member getter at this+0xd0. __fastcall this=ecx. pack(1) keeps the field at
// the retail byte offset (natural alignment would shift an odd one).
#pragma pack(push,1)
struct T {
    char pad[0xd0];
    float field;
    float Get();
};
#pragma pack(pop)
float T::Get() { return this->field; }
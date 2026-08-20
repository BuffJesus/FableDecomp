// Member getter at this+0x1c4. __fastcall this=ecx. pack(1) keeps the field at
// the retail byte offset (natural alignment would shift an odd one).
#pragma pack(push,1)
struct T {
    char pad[0x1c4];
    float field;
    float Get(int a0);
};
#pragma pack(pop)
float T::Get(int a0) { return this->field; }
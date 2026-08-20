// Member getter at this+0x194. __fastcall this=ecx. pack(1) keeps the field at
// the retail byte offset (natural alignment would shift an odd one).
#pragma pack(push,1)
struct T {
    char pad[0x194];
    float field;
    float Get(int a0);
};
#pragma pack(pop)
float T::Get(int a0) { return this->field; }
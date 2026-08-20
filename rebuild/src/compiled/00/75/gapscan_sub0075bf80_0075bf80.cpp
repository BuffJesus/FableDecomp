// Address-of-member accessor (`lea`) at this+0x80. __fastcall this=ecx.
#pragma pack(push,1)
struct Sub { int dummy; };
struct T {
    char pad[0x80];
    Sub field;
    Sub* GetAddr();
};
#pragma pack(pop)
Sub* T::GetAddr() { return &this->field; }
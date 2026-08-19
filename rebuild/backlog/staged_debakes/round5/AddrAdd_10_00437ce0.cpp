// `return (char*)this->sub + 0x28;` — address arithmetic on a sub-object pointer.
// __fastcall this=ecx, no args.
#pragma pack(push,1)
struct Sub { char pad[0x28]; int field; };
struct T { char pad[0x160]; Sub* sub; int* DoSizeof(); };
#pragma pack(pop)
int* T::DoSizeof() { return &this->sub->field; }

// Store -1 into a field of the sub-object pointer at this+0x248. __fastcall this=ecx.
#pragma pack(push,1)
struct Sub { char pad[0x2c]; int field; };
struct T { char pad[0x248]; Sub* sub; void Hide(); };
#pragma pack(pop)
void T::Hide() { this->sub->field = -1; }
